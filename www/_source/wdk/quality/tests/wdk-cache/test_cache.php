<?php

	require_once(GetWDKDir()."wdk_cache.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK Cache");
		}
		


		function TestCase_Cache($cache)
		{
			$cache->SetCacheObject("a","Alpha",10);
			$this->Trace("Set a cache object");
			if ($cache->GetCacheObject("a") != "Alpha")
			{
				$this->Trace("TESTCASE FAILED");			
				$this->SetResult(false);
				return;	
			}
			$cache->DeleteCacheObject("a");
			$this->Trace("Delete a cache object");
			if ($cache->GetCacheObject("a") != false)
			{
				$this->Trace("TESTCASE FAILED");			
				$this->SetResult(false);				
				return;	
			}
			
			$cache->SetCacheObject("a","Alpha",10);		
			$cache->SetCacheObject("b","Beta",10);		
			$this->Trace("Set two new cache objects");
			if ($cache->GetCacheObject("a") != "Alpha")
			{
				$this->Trace("TESTCASE FAILED");											
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("b") != "Beta")
			{
				$this->Trace("TESTCASE FAILED");											
				$this->SetResult(false);
				return;	
			}
			$cache->FlushCache();
			$this->Trace("Flush the cache");			
			if ($cache->GetCacheObject("a") != false)
			{
				$this->Trace("TESTCASE FAILED");							
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("b") != false)
			{
				$this->Trace("TESTCASE FAILED");			
				$this->SetResult(false);
				return;	
			}




			$cache->SetCacheObject("a","Alpha",0);		
			$cache->SetCacheObject("b","Beta",0);		
			$cache->SetCacheObject("c","Gamma",10);		
			$this->Trace("Set two new cache objects that are valid for 0 seconds, and one that is valid for 10 seconds, then sleep for 2 seconds");
			sleep(1);    
			if ($cache->GetCacheObject("a") != false)
			{
				$this->Trace("TESTCASE FAILED");											
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("b") != false)
			{
				$this->Trace("TESTCASE FAILED");											
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("c") != "Gamma")
			{
				$this->Trace("TESTCASE FAILED");											
				$this->SetResult(false);
				return;	
			}
			$cache->ClearCache();
			$this->Trace("Clear the cache (remove all expired entries)");			
			if ($cache->GetCacheObject("a") != false)
			{
				$this->Trace("TESTCASE FAILED");							
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("b") != false)
			{
				$this->Trace("TESTCASE FAILED");			
				$this->SetResult(false);
				return;	
			}
			if ($cache->GetCacheObject("c") != "Gamma")
			{
				$this->Trace("TESTCASE FAILED");			
				$this->SetResult(false);
				return;	
			}
			
			
			$this->Trace("TESTCASE PASSED");			
		}


		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$cache = new CInstanceMemoryCache();
			$this->TestCase_Cache($cache);
			
			
			
			
					

			
		}
		
		
	}
	
	

		
