<?php

	require_once(GetWDKDir().'wdk_list.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ParseURL');
		}
		

		function TestCase_ParseURL(
			$strURL,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ParseURL');
	
			$this->Trace('URL: '.$strURL);
			if (is_array($arrayExpectedResult))
			{
				$this->Trace('Expected Result:');
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace('Expected Result: '.RenderBool($arrayExpectedResult));
			}

			$arrayResult = ParseURL($strURL);
			
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
			
			$strURL = 'http://www.example.com/test/?tag=value';
			$arrayURL = array(
				'PROTOCOL' => 'http',
				'HOST' => 'www.example.com',
				'PORT' => '',
				'USER' => '',
				'PASSWORD' => '',
				'PATH' => '/test/',
				'PARAMETERS' => 'tag=value',
				'ANCHOR' => ''
				);
			$this->TestCase_ParseURL($strURL,$arrayURL);
			$this->TestCase_ParseURL(u($strURL),$arrayURL);

			$strURL = 'https://me:secret@www.example.com:80/test/?tag=value#section2';
			$arrayURL = array(
				'PROTOCOL' => 'https',
				'HOST' => 'www.example.com',
				'PORT' => '80',
				'USER' => 'me',
				'PASSWORD' => 'secret',
				'PATH' => '/test/',
				'PARAMETERS' => 'tag=value',
				'ANCHOR' => 'section2'
				);
			$this->TestCase_ParseURL($strURL,$arrayURL);
			$this->TestCase_ParseURL(u($strURL),$arrayURL);
			
			
			$strURL = 'blubb://me:secret@www.example.com:80/test/?tag=value#section2';
			$arrayURL = false;
			$this->TestCase_ParseURL($strURL,$arrayURL);
			$this->TestCase_ParseURL(u($strURL),$arrayURL);			
				
			$strURL = 'http://www.youtube.com/watch?v=kNuW5yDaxTY&feature=popular';
			$arrayURL = array(
				'PROTOCOL' => 'http',
				'HOST' => 'www.youtube.com',
				'PORT' => '',
				'USER' => '',
				'PASSWORD' => '',
				'PATH' => '/watch',
				'PARAMETERS' => 'v=kNuW5yDaxTY&feature=popular',
				'ANCHOR' => ''
				);
			$this->TestCase_ParseURL($strURL,$arrayURL);
			$this->TestCase_ParseURL(u($strURL),$arrayURL);

		}
	}
	
	

		
