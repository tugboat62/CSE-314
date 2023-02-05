#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->count_lock = PTHREAD_MUTEX_INITIALIZER;
  rw->reader_cv = PTHREAD_COND_INITIALIZER;
  rw->writer_cv = PTHREAD_COND_INITIALIZER;
  rw->reader_count = 0;
  rw->writer_lock = 0;
  rw->writer_queue_count = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&rw->count_lock);

  // wait while there is Writer-Lock or some thread is waiting for Writer-Lock
  while (rw->writer_lock || rw->writer_queue_count)
    pthread_cond_wait(&rw->reader_cv, &rw->count_lock);
  
  // Acquire Reader-Lock
  rw->reader_count++;

  pthread_mutex_unlock(&rw->count_lock);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&rw->count_lock);
  rw->reader_count--;
  
  // Try Writer-Lock if no more Reader-Lock
  if (rw->reader_count == 0)
    pthread_cond_signal(&rw->writer_cv);

  pthread_mutex_unlock(&rw->count_lock);
}

void WriterLock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&rw->count_lock);

  // Enqueue for Writer-Lock
  rw->writer_queue_count++;
  
  // Wait for no Reader-Lock and Writer-Lock
  while (rw->reader_count || rw->writer_lock)
    pthread_cond_wait(&rw->writer_cv, &rw->count_lock);
  
  // Acquire Reader-Lock
  rw->writer_lock = 1;

  pthread_mutex_unlock(&rw->count_lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&rw->count_lock);
  
  // Remove Writer-Lock
  rw->writer_queue_count--;
  rw->writer_lock = 0;

  // Try other Writer-Lock
  if (rw->writer_queue_count != 0)
    pthread_cond_signal(&rw->writer_cv);
  // Else try Reader-Lock
  else
    pthread_cond_broadcast(&rw->reader_cv);

  pthread_mutex_unlock(&rw->count_lock);
}
