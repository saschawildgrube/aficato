<?php

	require_once(GetWDKDir().'wdk_cache.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK CFileCache::ParseCacheObjectFileName');
		}
		

		function TestCase_ParseCacheObjectFileName(
			$strFileName,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ParseCacheObjectFileName');
	
			$this->Trace('File name: '.$strFileName);
			if (is_array($arrayExpectedResult))
			{
				$this->Trace('Expected Result:');
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace('Expected Result: '.RenderBool($arrayExpectedResult));
			}

			$cache = new CFileCache(GetTempDir(),'test');
			$arrayResult = $cache->ParseCacheObjectFileName($strFileName);
			
			if (is_array($arrayResult))
			{
				$this->Trace('Result:');
				$this->Trace($arrayResult);
			}
			else
			{
				$this->Trace('Result: '.RenderBool($arrayResult));
			}
			
		
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace('Testcase FAILED!');	
				$this->Trace('');
				$this->Trace('');
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace('Testcase PASSED!');
			$this->Trace('');
			$this->Trace('');
			
		}



		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);

			$strFileName = '';
			$arrayFileInfo = false;
			$this->TestCase_ParseCacheObjectFileName($strFileName,$arrayFileInfo);

			
			$strFileName = 'cache_test_2020-04-27-10-15-11_object.cache';
			$arrayFileInfo = array(
				'scope' => 'test',
				'expiry' => '2020-04-27 10:15:11',
				'id' => 'object'
				);
			$this->TestCase_ParseCacheObjectFileName($strFileName,$arrayFileInfo);


		}
	}
	
	

		
