#include "threadpool.h"

struct threadpool* init_pool(int imaxnum,int imaxsleepnum,int iminsleepnum)
{
	struct threadpool *pool = new struct threadpool();
	pthread_mutex_init(&pool->jobs_lock,NULL);
	pthread_cond_init(&pool->jobs_cond,NULL);
	pool->shutdown = 0;
	pool->maxnum = imaxnum;
	pool->maxsleepnum = imaxsleepnum;
	pool->minsleepnum = iminsleepnum;
	pool->add_thread = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(pool->add_thread,NULL,add_thread_runtine,pool);
	return pool;
}

void add_job(struct threadpool *pool,void(*process)(void*arg),void *arg)
{
	struct job* newjob = (struct job*)malloc(sizeof(struct job));
	newjob->job = process;
	newjob->arg = arg;
	pthread_mutex_lock(&(pool->jobs_lock));
	pool->jobs.push_back(newjob);
	pthread_mutex_unlock(&(pool->jobs_lock));
	pthread_cond_signal(&(pool->jobs_cond));
}

void *add_thread_runtine(void *arg)
{
	struct threadpool* pool = (struct threadpool*)arg;
	while(1)
	{
		pthread_mutex_lock(&(pool->jobs_lock));

		if(pool->shutdown)	
		{
			//cout<<"bbbbbbbbbb"<<endl;
			pthread_mutex_unlock(&pool->jobs_lock);
			pthread_exit(NULL);
		}

		int working = 0;
		int total = pool->mapthread.size();
		map<pthread_t*,int>::iterator map_it = pool->mapthread.begin();
		for(;map_it != pool->mapthread.end();++map_it)	//get the number of running thread
		{
			if(map_it->second == 1)
				working++;
		}
		if((total - working) < pool->minsleepnum)	//create thread if cursleepnum smaller than minsleepnum
		{
			pthread_t * newthread = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_create(newthread,NULL,thread_runtine,pool);
			pool->mapthread.insert(make_pair(newthread,1));	
		}			
	
		pthread_mutex_unlock(&(pool->jobs_lock));
	}
}

void *thread_runtine(void *arg)
{
	struct threadpool* pool = (struct threadpool*)arg;
	//cout<<"total thread:"<<pool->mapthread.size()<<endl;
	//cout<<"starting thread 0x"<<pthread_self()<<endl;
	while(1)
	{
		pthread_mutex_lock(&(pool->jobs_lock));
		int working = 0;
		int total = pool->mapthread.size();
		map<pthread_t*,int>::iterator map_it = pool->mapthread.begin();
		for(;map_it != pool->mapthread.end();++map_it)		//get the number of running thread
		{
			if(map_it->second == 1)
				working++;
		}

		if((total - working) >= pool->maxsleepnum)	//exit if too many sleep thread;
		{
			cout<<"too many thread, 0x"<<pthread_self()<<" will exit"<<endl;
			map_it = pool->mapthread.begin();
			for (;map_it != pool->mapthread.end(); ++map_it)	//find the this thread's iterator in pool->mapthread
			{
				if(*(map_it->first) == pthread_self())
					break;
			}
			if(map_it != pool->mapthread.end())
				pool->mapthread.erase(map_it);		//delete this pthread form pool's mapthread

			pthread_mutex_unlock(&(pool->jobs_lock));
			pthread_exit(NULL);
		}
		while((pool->jobs.size()==0)&&(pool->shutdown == 0))	//condition wait if no job in jobs
		{
			cout<<"thread 0x"<<pthread_self()<<"  is waitting"<<endl;
			map_it = pool->mapthread.begin();
			for (;map_it != pool->mapthread.end(); ++map_it)		//find this thread in pool->mapthread
			{
				if(*(map_it->first) == pthread_self())
					break;
			}
			map_it->second = 0;		//set to sleep
			pthread_cond_wait(&(pool->jobs_cond),&(pool->jobs_lock));
		}

		if(pool->shutdown == 1)		//exit if the object is destroy
		{
			cout<<"thread 0x"<<pthread_self()<<"will exit"<<endl;
			map_it = pool->mapthread.begin();
			for (;map_it != pool->mapthread.end(); ++map_it)	//find the this thread's iterator in pool->mapthread
			{
				if(*(map_it->first) == pthread_self())
					break;
			}
			if(map_it != pool->mapthread.end())
				pool->mapthread.erase(map_it);		//delete this pthread form pool's mapthread
			pthread_mutex_unlock(&(pool->jobs_lock));
			pthread_exit(NULL);
		}

		if(pool->jobs.size() != 0)
		{
			struct job *newjob = pool->jobs.front();	//get job from jobs's front
			pool->jobs.erase(pool->jobs.begin());		//delete the first job
			pthread_mutex_unlock(&(pool->jobs_lock));	

			(*newjob->job)(newjob->arg);		//do the job

			//delete((string *)newjob->arg);	//delete the argument string
			//free(newjob);		
		}
	}
}

void destroy_pool(struct threadpool *pool)
{
	if(pool->shutdown)	//if it is shutting down then return
		return;
	pool->shutdown = 1;	//shutdown
	pthread_cond_broadcast(&(pool->jobs_cond));		//wakeup all thread to destroy
	pthread_join(*pool->add_thread,NULL);
	map<pthread_t*,int>::iterator map_it = pool->mapthread.begin();
	for(;map_it != pool->mapthread.end(); ++map_it)
	{	
		pthread_join(*(map_it->first),NULL);	//wait each thread to exit
		free(map_it->first);
	}

	for(vector<job*>::iterator iter=pool->jobs.begin();iter != pool->jobs.end();++iter)
	{
		//cout<<"aaaaaaaaa"<<endl;
		delete((string*)(*iter)->arg);		//delete exist job's argument
	}
	pthread_mutex_destroy(&(pool->jobs_lock));	//destroy jobs_lock;
	pthread_cond_destroy(&(pool->jobs_cond));	//destroy jobs_cond;
	delete(pool);	//????????????????fail to delete pool???????????
	//cout<<"aaaaaa"<<endl;
}