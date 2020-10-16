<?php

	require_once(GetWDKDir().'wdk_cache.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK Cache');
		}

		function TestCase_Cache($cache)
		{
			$this->Trace('FlushCache');
			$cache->FlushCache();
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 0)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an empty array.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			
			$this->Trace('Set a cache object');
			if ($cache->SetCacheObject('a','Alpha',10) != true)
			{
				$this->Trace('SetCacheObject returned false.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			$this->Trace($arrayCacheObjectInfos);
			$this->Trace(RenderValue($cache->GetCacheObject('a')));
			if ($cache->GetCacheObject('a') != 'Alpha')
			{
				$this->Trace('GetCacheObject did not return "Alpha".');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 1)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an array of 1.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			
			$this->Trace('Delete a cache object');
			$cache->DeleteCacheObject('a');
			if ($cache->GetCacheObject('a') != false)
			{
				$this->Trace('GetCacheObject did not return false.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);				
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 0)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an empty array.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			
			$this->Trace('Set two new cache objects');
			$cache->SetCacheObject('a','Alpha',10);		
			$cache->SetCacheObject('b','Beta',10);		
			if ($cache->GetCacheObject('a') != 'Alpha')
			{
				$this->Trace('GetCacheObject(a) did not return "Alpha".');
				$this->Trace('TESTCASE FAILED');											
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('b') != 'Beta')
			{
				$this->Trace('GetCacheObject(b) did not return "Beta".');
				$this->Trace('TESTCASE FAILED');											
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 2)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an array of 2.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
		
			$this->Trace('Flush the cache');			
			$cache->FlushCache();
			if ($cache->GetCacheObject('a') != false)
			{
				$this->Trace('GetCacheObject(a) did not return false.');
				$this->Trace('TESTCASE FAILED');							
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('b') != false)
			{
				$this->Trace('GetCacheObject(b) did not return false.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 0)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an empty array.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}




			$this->Trace('Set two new cache objects that are valid for 0 seconds, and one that is valid for 10 seconds, then sleep for 2 seconds');
			$cache->SetCacheObject('a','Alpha',0);		
			$cache->SetCacheObject('b','Beta',0);		
			$cache->SetCacheObject('c','Gamma',10);		
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 3)
			{
				$this->Trace('GetAllCacheObjectInfos did not an array of 3.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			sleep(2);    
			if ($cache->GetCacheObject('a') != false)
			{
				$this->Trace('GetCacheObject("a") did not return false.');
				$this->Trace('TESTCASE FAILED');											
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			//$this->Trace($arrayCacheObjectInfos);
			if (ArrayCount($arrayCacheObjectInfos) != 2)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an array of 2.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('b') != false)
			{
				$this->Trace('GetCacheObject("b") did not return false.');
				$this->Trace('TESTCASE FAILED');											
				$this->SetResult(false);
				return;	
			}
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 1)
			{
				$this->Trace('GetAllCacheObjectInfos did not an array of 1.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('c') != 'Gamma')
			{
				$this->Trace('GetCacheObject("c") did not return "Gamma".');
				$this->Trace('TESTCASE FAILED');											
				$this->SetResult(false);
				return;	
			}
			
			
			$this->Trace('Set two new cache objects that are valid for 0 seconds, then sleep for 2 seconds');
			$cache->SetCacheObject('a','Alpha',0);		
			$cache->SetCacheObject('b','Beta',0);		
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 3) // because of cache object c
			{
				$this->Trace('GetAllCacheObjectInfos did not an array of 3.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}

			$this->Trace('Clear the cache (remove all expired entries)');			
			$cache->ClearCache();
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 1) // because of cache object c
			{
				$this->Trace('GetAllCacheObjectInfos did not an array of 1.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('a') != false)
			{
				$this->Trace('GetCacheObject("a") did not return false.');
				$this->Trace('TESTCASE FAILED');							
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('b') != false)
			{
				$this->Trace('GetCacheObject("b") did not return false.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject('c') != 'Gamma')
			{
				$this->Trace('GetCacheObject("c") did not return "Gamma".');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}
			
			$cache->FlushCache();
			$arrayCacheObjectInfos = $cache->GetAllCacheObjectInfos();
			if (ArrayCount($arrayCacheObjectInfos) != 0)
			{
				$this->Trace('GetAllCacheObjectInfos did not return an empty array.');
				$this->Trace('TESTCASE FAILED');			
				$this->SetResult(false);
				return;	
			}

			
			$this->Trace('TESTCASE PASSED');			
		}


		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->Trace('Testing CMemoryCache');
			$cache = new CMemoryCache();
			$this->TestCase_Cache($cache);
			$this->Trace('');
			$this->Trace('');		



			$this->Trace('Testing CFileCache');
			$this->Trace('GetTempDir(): '.GetTempDir());
			if (IsDirectoryReadWriteAccess(GetTempDir()))
			{
				$cache = new CFileCache(GetTempDir());
				$this->Trace('CFileCache::GetCacheDirectory(): '.$cache->GetCacheDirectory());
				if ($cache->IsReady() == false)
				{
					$this->Trace('CFileCache::IsReady() returned false');			
					$this->SetResult(false);	
				}
				else
				{
					$this->TestCase_Cache($cache);
				}
			}	
			else
			{
				$this->Trace('CFileCache is not tested, because GetTempDir() does not return a directory with read and write access.');	
			}
			$this->Trace('');
			$this->Trace('');		


		}
	}
	
	

		
