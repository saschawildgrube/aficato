<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringEncodeUTF8");
		}
		

		function TestCase_StringEncodeUTF8(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_StringEncodeUTF8");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
			$strResult = StringEncodeUTF8($strParam);
	
			$this->Trace("strResult = \"$strResult\"");
	
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
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$this->TestCase_StringEncodeUTF8("Ä",mb_convert_encoding("Ä", 'UTF-8', 'ISO-8859-1'));
			$this->TestCase_StringEncodeUTF8("ÄÖÜ",mb_convert_encoding("ÄÖÜ", 'UTF-8', 'ISO-8859-1'));
			$this->TestCase_StringEncodeUTF8("ÄÖÜ".mb_convert_encoding("ÄÖÜ", 'UTF-8', 'ISO-8859-1'),mb_convert_encoding("ÄÖÜÄÖÜ", 'UTF-8', 'ISO-8859-1'));
			$this->TestCase_StringEncodeUTF8(mb_convert_encoding("ÄÖÜ", 'UTF-8', 'ISO-8859-1'),mb_convert_encoding("ÄÖÜ", 'UTF-8', 'ISO-8859-1'));
			$this->TestCase_StringEncodeUTF8("ABC","ABC");
			$this->TestCase_StringEncodeUTF8(
				"A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ",
				mb_convert_encoding("A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ", 'UTF-8', 'ISO-8859-1'));
		}
		
		
	}
	
	

		
