<?php

	require_once(GetWDKDir().'wdk_url.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test IsValidURL');
		}
		
		function OnInit()
		{
			parent::OnInit();
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

		
		function OnTest()
		{
			parent::OnTest();
			
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
 			$this->TestCase_IsValidURL('https://www.example.com?',true);
 			$this->TestCase_IsValidURL('https://www.example.com/?',true);
 			$this->TestCase_IsValidURL('https://www.example.com/test/?',true);
 			$this->TestCase_IsValidURL('https://www.example.com?#',true);
 			$this->TestCase_IsValidURL('https://www.example.com/?#',true);
 			$this->TestCase_IsValidURL('https://www.example.com/test/?#',true);
 			$this->TestCase_IsValidURL('http://fonts.googleapis.com/css?family=Montserrat%3A400%2C700',true);
 			$this->TestCase_IsValidURL('http://fonts.googleapis.com/css?family=Open+Sans%3A300italic%2C400italic%2C600italic%2C700italic%2C800italic%2C400%2C300%2C600%2C700%2C800',true);
 			$this->TestCase_IsValidURL('http://www.cio.de/a/die-10-wichtigsten-technologie-trends-2016,3248934.html',true);
 			$this->TestCase_IsValidURL('https://www.amazon.de/Game-Penetrating-Secret-Society-Artists/dp/0061240168/ref=sr_1_6?ie=UTF8&qid=1473444983&sr=8-6&keywords=the+game',true);
 			$this->TestCase_IsValidURL('https://analytics.google.com/analytics/web/#embed/report-home/a7THIS46IS10A79TESTp114482491/',true);
 			$this->TestCase_IsValidURL('https://store.servicenow.com/sn_appstore_store.do#!/store/application/f13e0cce0fabaa003e0b87ece1050ec3/2.0.0?referer=%2Fstore%2Fsearch%3Flistingtype%3Dallintegrations%25253Bancillary_app%25253Bcertified_apps%25253Bcontent%25253Bindustry_solution%25253Boem%25253Butility%26q%3Dfile&sl=sh',true);
			$this->TestCase_IsValidURL('https://this-is-my.sharepoint.com/:p:/p/john_Doe/ETDr3RtNrenxTv_gBZVXHzsmHOwU2bJQeQ?e=gVdI0a',true);
			$this->TestCase_IsValidURL('https://services.service-now.com/nav_to.do?uri=%2Frm_story_list.do%3Fsysparm_fixed_query%3Dsys_class_name%3Drm_story%26sysparm_view%3Dscrum%26sysparm_first_row%3D1%26sysparm_query%3Dactive%3Dtrue%5Esys_domain%3Djavascript:gs.getUser().getDomainID();%5Eproduct.display_nameSTARTSWITHHello@World%5Eepic.parent_epic.numberSTARTSWITHEPIC0000000%26sysparm_clear_stack%3Dtrue',true);
			$this->TestCase_IsValidURL('https://servicenow.sharepoint.com/:x:/r/sites/HelloWorld/_layouts/15/doc2.aspx?sourcedoc=%7B81dcd5cf-0832-459e-914a-d025273f7a9f%7D&action=edit&activeCell=%27Action%20items%27!B25&wdinitialsession=a223cbcf-1ff9-49a5-9910-459dc4c951be&wdrldsc=3&wdrldc=1&wdrldr=AccessTokenExpiredWarning&cid=9578f9a6-ff14-4791-9a17-9b9667eaf038&wdLOR=c3A4FA22B-7EC4-2686-9A24-AB25C196A043&CID=0914377F-1523-AF46-8CE1-7288FA1AB6CC',true);
			$this->TestCase_IsValidURL('https://consent.google.com/ml?continue=https://www.google.com/maps/place/Hello%2BWorld/@1.2345,2.2345,10z/data%3D!4m18!1m9!3m8!1s0x478fa1053c87a8bc:0x537d8e8f30277fbd!2sHello%2BWorld!5m2!4m1!1i2!8m2!3d46.5932396!4d1.234567!3m7!1s0x478fa1053c87a866:0x537d8e8f30277fbd!5m2!4m1!1i2!8m2!3d1.23456!4d2.3456?hl%3Dde&gl=DE&m=0&pc=m&hl=de&src=1',true);
			$this->TestCase_IsValidURL('https://servicenow.highspot.com/items/aa680zz1c714334388233c9c?lfrm=srp.1#1',true);


 			
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
			$this->TestCase_IsValidURL('HTTP://www.example.com',false);
			$this->TestCase_IsValidURL('HTTPS://www.example.com',false);
			$this->TestCase_IsValidURL('FTP://www.example.com',false);
		
			
		}
		
		
	}
	
	

		
