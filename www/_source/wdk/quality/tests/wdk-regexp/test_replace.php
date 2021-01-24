<?php
	
	
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RegExpReplace");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_RegExpReplace($strSubject,$strPattern,$strReplace,$strExpectedResult)
		{
			$this->Trace("TestCase_RegExpReplace");
			$this->Trace("strSubject = \"$strSubject\"");
			$this->Trace("Pattern = \"$strPattern\"");
			$this->Trace("Replace = \"$strReplace\"");
			$this->Trace("Expected Result: \"".$strExpectedResult."\"");
			$strResult = RegExpReplace($strSubject,$strPattern,$strReplace);
			$this->Trace("RegExpReplace returns: \"".$strResult."\"");
			if ($strResult == $strExpectedResult)
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
		
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->TestCase_RegExpReplace("Hello World","/Wor/","WOR","Hello WORld");
			$this->TestCase_RegExpReplace("Hello World","/wor/","WOR","Hello World");
			$this->TestCase_RegExpReplace("Hello World","/wor/i","WOR","Hello WORld");
			$this->TestCase_RegExpReplace("Hello World","wor","WOR",""); // invalid patterns lead to an empty result
			
			$this->TestCase_RegExpReplace(
				'This<img src="image1.png"/>is<img src="image2.png>a<IMG>test!',
				'/<img[^>]*>/', 
				' ',
				"This is a<IMG>test!");

			$this->TestCase_RegExpReplace(
				'This<img src="image1.png"/>is<img src="image2.png>a<IMG>test!',
				'/<img[^>]*>/i',   
				' ',
				"This is a test!");

			
		}

	}
	
	

		
