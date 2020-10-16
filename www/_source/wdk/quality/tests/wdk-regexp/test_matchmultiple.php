<?php
	
	require_once(GetWDKDir()."wdk_mail.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RegExpMatchMultiple");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_RegExpMatchMultiple($strSubject,$strPattern,$bIncludePositions,$arrayExpectedResult)
		{
			$this->Trace("TestCase_RegExpMatch");
			$this->Trace("Subject = \"$strSubject\"");
			$this->Trace("Pattern = \"$strPattern\"");
			$this->Trace("IncludePositions = ".RenderBool($bIncludePositions));
			$this->Trace("Expected Result: ");
			$this->Trace($arrayExpectedResult);
			$arrayResult = RegExpMatchMultiple($strSubject,$strPattern,$bIncludePositions);
			$this->Trace("RegExpMatchMultiple returns: ");
			$this->Trace($arrayResult);
			if ($arrayResult == $arrayExpectedResult)
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
			
			$this->TestCase_RegExpMatchMultiple("abc@def.com and xyz@test.de","/".StringSection(REGEXP_EMAIL,2,-2)."/",false,   
				array(
					"abc@def.com",
					"xyz@test.de")
				);

			$this->TestCase_RegExpMatchMultiple("Is A(123) and B(123) the same?",'^A\(([a-z0-9]+)\)^',false,     
				array(
					"A(123)")
				);

			$this->TestCase_RegExpMatchMultiple("stuff UnCryptMailto('acknvq,gtpuvBqji/hwtvycpigp0fg'); UnCryptMailto('bcknvq,gtpuvBqji/hwtvycpigp0fg'); more stuff",'^UnCryptMailto\(\'([a-zA-B0-9,\/]+)\'\);^',false,       
				array(
					"UnCryptMailto('acknvq,gtpuvBqji/hwtvycpigp0fg');",
					"UnCryptMailto('bcknvq,gtpuvBqji/hwtvycpigp0fg');")
				);

				
		}
		

	}
	
	

		
