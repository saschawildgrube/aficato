<?php
	
	
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RegExpMatch");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_RegExpMatch($strSubject,$strPattern,$bExpectedResult)
		{
			$this->Trace("TestCase_RegExpMatch");
			$this->Trace("strSubject = \"$strSubject\"");
			$this->Trace("strPattern = \"$strPattern\"");
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult));
			$bResult = RegExpMatch($strSubject,$strPattern);
			$this->Trace("RegExpMatch returns: ".RenderBool($bResult));
			if ($bResult == $bExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_RegExpMatch("Hello World","/Wor/",true);
			$this->TestCase_RegExpMatch("Hello World","/Word/",false);
			$this->TestCase_RegExpMatch("Hello World","/ello\sW/",true);
			$this->TestCase_RegExpMatch("Hello World","/ello\s/",true);
			$this->TestCase_RegExpMatch("Hello World","/elo\s/",false);
			$this->TestCase_RegExpMatch("\$this->Lo"."g(\"INFORMATIONAL\")","/->"."Log\(/",true);  
			$this->TestCase_RegExpMatch("2010-02-26","/^([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})$/",true);
			$this->TestCase_RegExpMatch("   2010-02-26     ","/([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})/",true);
			$this->TestCase_RegExpMatch("2010-02-26 00:23","/([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})/",true);
			$this->TestCase_RegExpMatch("2010-2-26 00:23","/([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})/",false);
			$this->TestCase_RegExpMatch("2010-02","/([0-9]{4})([-]{1})([0-9]{2})-([0-9]{2})/",false);
			$this->TestCase_RegExpMatch("abcdef","/abc/",true);
			$this->TestCase_RegExpMatch("abcdef","/cba/",false);
			$this->TestCase_RegExpMatch(u("äbcdef"),u("/äbc/"),true);
			$this->TestCase_RegExpMatch(u("äbcdef"),u("/cbä/"),false);
			$this->TestCase_RegExpMatch('Folder (123)','/\([0-9]+\)/m',true);
			$this->TestCase_RegExpMatch('Folder ()','/\([0-9]+\)/m',false);
			$this->TestCase_RegExpMatch('Folder','/\([0-9]+\)/m',false);
			$this->TestCase_RegExpMatch('webservice_default_accesscode','/webservice.*accesscode/',true);
		}
		

	}
	
	

		
