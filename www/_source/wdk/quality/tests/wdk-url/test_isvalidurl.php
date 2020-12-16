<?php

	require_once(GetWDKDir().'wdk_url.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test IsValidURL');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidURL(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace('TestCase_IsValidURL');
		
			$bValue = IsValidURL($value);
		
			$this->Trace('IsValidURL('.RenderValue($value).') = '.RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
			}
			$this->Trace('');
			$this->Trace('');
		}

		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->Trace('GetRegExURL():');
			$this->Trace(GetRegExURL());
			$this->Trace('');

			$this->TestCase_IsValidURL('http://localhost',true);
			$this->TestCase_IsValidURL('http://localhost:8080',true);
			$this->TestCase_IsValidURL('http://localhost/',true);
			$this->TestCase_IsValidURL('https://localhost',true);
			$this->TestCase_IsValidURL('https://localhost/',true);
			$this->TestCase_IsValidURL('http://192.168.1.1',true);
			$this->TestCase_IsValidURL('http://192.168.1.1:80',true);
			$this->TestCase_IsValidURL('http://192.168.1.1/',true);
			$this->TestCase_IsValidURL('https://192.168.1.1',true);
			$this->TestCase_IsValidURL('https://192.168.1.1/',true);
			$this->TestCase_IsValidURL('http://www.example.com',true);
			$this->TestCase_IsValidURL('https://www.websitedevkit.com',true);
			$this->TestCase_IsValidURL('ftp://ftp.example.com',true);
			$this->TestCase_IsValidURL('https://user:pass@www.somewhere.com:8080/login.php?do=login&style=%23#pagetop',true);
 			$this->TestCase_IsValidURL('http://user@www.somewhere.com/#pagetop',true);
 			$this->TestCase_IsValidURL('https://somewhere.com/index.html',true);
 			$this->TestCase_IsValidURL('ftp://user:pass@somewhere.com:21/',true);
 			$this->TestCase_IsValidURL('http://somewhere.com/index.html/',true);
 			$this->TestCase_IsValidURL('http://video.google.com/videoplay?docid=514425911425024420#',true);
 			$this->TestCase_IsValidURL('http://www.youtube.com/watch?v=kNuW5yDaxTY&feature=popular',true);
 			$this->TestCase_IsValidURL('http://en.wikipedia.org/wiki/Connascence_(computer_programming)',true);
 			$this->TestCase_IsValidURL('http://en.wikipedia.org/wiki/Nassi%E2%80%93Shneiderman_diagram',true);
 			$this->TestCase_IsValidURL('https://docs.google.com/spreadsheets/d/1m1t4BNbg8cS8jkcAKXcTHIStaISglA7sTESTAoMR5fc/edit#gid=0',true); 			
 			$this->TestCase_IsValidURL('https://www.example.com/test1//test2/',true);
 			$this->TestCase_IsValidURL('https://www.example.com/test1=1/',true);
 			$this->TestCase_IsValidURL('https://www.example.com/test1_1/',true);
 			$this->TestCase_IsValidURL('http://fonts.googleapis.com/css?family=Montserrat%3A400%2C700',true);
 			$this->TestCase_IsValidURL('http://fonts.googleapis.com/css?family=Open+Sans%3A300italic%2C400italic%2C600italic%2C700italic%2C800italic%2C400%2C300%2C600%2C700%2C800',true);
 			$this->TestCase_IsValidURL('http://www.cio.de/a/die-10-wichtigsten-technologie-trends-2016,3248934.html',true);
 			$this->TestCase_IsValidURL('https://www.amazon.de/Game-Penetrating-Secret-Society-Artists/dp/0061240168/ref=sr_1_6?ie=UTF8&qid=1473444983&sr=8-6&keywords=the+game',true);
 			$this->TestCase_IsValidURL('https://analytics.google.com/analytics/web/#embed/report-home/a7THIS46IS10A79TESTp114482491/',true);
 			$this->TestCase_IsValidURL('https://store.servicenow.com/sn_appstore_store.do#!/store/application/f13e0cce0fabaa003e0b87ece1050ec3/2.0.0?referer=%2Fstore%2Fsearch%3Flistingtype%3Dallintegrations%25253Bancillary_app%25253Bcertified_apps%25253Bcontent%25253Bindustry_solution%25253Boem%25253Butility%26q%3Dfile&sl=sh',true);

 			
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#managemembers/hello-world/members/active',true);
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#manage!members/hello-world/members/active',true);
 			
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test.com/forum/#!managemembers/hello-world/members/active',true);
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#!managemembers/hello-world/members/active',true);
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#!managemembers',true);
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#!',true);
 			$this->TestCase_IsValidURL('https://groups.google.com/a/test/forum/#',true);
 			
 			$this->TestCase_IsValidURL('https://docs.google.com/spreadsheets/d/123-ABC-abc/edit#gid=12345&fvid=6789',true);
 			
 			$this->TestCase_IsValidURL('https://www.example.com/index.html#/folder/endpoint',true);
 			$this->TestCase_IsValidURL('http://www.example.org/~folder/subfolder/',true);
 			$this->TestCase_IsValidURL('http://www.example.org/@folder/',true);
 			$this->TestCase_IsValidURL('http://www.example.org/@folder/subfolder/',true);
 			
 			$this->TestCase_IsValidURL('http://www.example.com/?hello?world',true);
 			$this->TestCase_IsValidURL('http://www.example.com/?param={hello-world}',true);
 			
 			$this->TestCase_IsValidURL('https://www.example.com/#/hello/World/HelloWorld?:value=1',true);
 			$this->TestCase_IsValidURL('https://www.example.com/#/?:hello=world',true);
 			$this->TestCase_IsValidURL('https://www.example.com/?:hello=world',true);
 			$this->TestCase_IsValidURL('https://www.example.com/:path:/',true);
 			

			$this->TestCase_IsValidURL('',false);
			$this->TestCase_IsValidURL('http://',false);
			$this->TestCase_IsValidURL('http://a',false);
			$this->TestCase_IsValidURL('123',false);
			$this->TestCase_IsValidURL('198.168.1.1',false);
			$this->TestCase_IsValidURL('http://198.168.1.256',false);
			$this->TestCase_IsValidURL('http://locallhost',false);
			$this->TestCase_IsValidURL('http://localhos',false);
			$this->TestCase_IsValidURL('localhost',false);
			$this->TestCase_IsValidURL(1,false);
			$this->TestCase_IsValidURL(false,false);
			$this->TestCase_IsValidURL('http:www.example.com',false);
			$this->TestCase_IsValidURL(u('http://www.exämple.com'),false);
			$this->TestCase_IsValidURL('http://http://www.example.com',false);

		
			
		}
		
		
	}
	
	

		
