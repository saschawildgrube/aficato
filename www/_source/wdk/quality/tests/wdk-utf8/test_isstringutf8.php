<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("IsStringUTF8");
		}
		

		function TestCase_IsStringUTF8(
			$strParam,
			$bExpectedResult)
		{ 
			$this->Trace("TestCase_IsStringUTF8");
	
			$this->Trace("strParam        = \"$strParam\"");
			$this->Trace("bExpectedResult = \"".RenderBool($bExpectedResult)."\"");
			$sw = new CStopWatch();
			$sw->Start();
			$bResult = IsStringUTF8($strParam);
			$sw->Stop();
			$this->Trace("bResult = \"".RenderBool($bResult)."\"");
			if ($bResult == $bExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("Elapsed time (secs): ".$sw->GetSeconds());
			$this->Trace("");
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			$this->TestCase_IsStringUTF8("Ä",false);
			$this->TestCase_IsStringUTF8(utf8_encode("Ä"),true);
			$this->TestCase_IsStringUTF8("ABC",true);
			$this->TestCase_IsStringUTF8("ÄÜÖÜÄäöü",false);
			$this->TestCase_IsStringUTF8(utf8_encode("äöüÄÖÜ"),true);
			$this->TestCase_IsStringUTF8("ÄÜÖÜÄäöü".utf8_encode("ÄÄ")."äöü",false);
			$this->TestCase_IsStringUTF8("A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ",false);			
			$this->TestCase_IsStringUTF8(utf8_encode("A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ"),true);
			
			
			$strInput = "";
			for ($nIndex = 0; $nIndex < 500; $nIndex++)
			{
				$strInput .= utf8_encode("ÄÖÜ_$nIndex\n");	
			}
			$this->TestCase_IsStringUTF8($strInput,true);

		}
		
		
	}
	
	

		
