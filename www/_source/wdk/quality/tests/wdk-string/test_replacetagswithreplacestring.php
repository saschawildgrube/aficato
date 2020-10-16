<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReplaceTagsWithReplaceString");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ReplaceTagsWithReplaceString(
			$strTemplate,
			$arrayTags,
			$strTagPrefix,
			$strTagPostfix,
			$strSearch,
			$strReplace,
			$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceTagsWithReplaceString");
			$this->Trace("strTemplate:");
			$this->Trace($strTemplate);
			$this->Trace("Expected Result:");
			$this->Trace($strExpectedResult);
			$sw = new CStopWatch();
			$sw->Start();
			$strResult = ReplaceTagsWithReplaceString(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strSearch,
				$strReplace);
			$sw->Stop();
			$this->Trace("ReplaceTagsWithReplaceString returns:");
			$this->Trace($strResult);
			if ($strResult == $strExpectedResult)
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
			
		}

		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$arrayTags = array(
				"TID_ITEM1" => "blubb\nand a new line.",
				"TID_WORLD" => "World",
				"TID_NAME" => "John",
				"TID_TODAY" => "today");
			$strTemplate =
"Loram ipsim ?TID_ITEM1?<br/>
Hallo ?TID_WORLD?! Hello ?TID_WORLD?<br/>
Plusquam dolom.<br/>
Hello ?TID_NAME?, how are you ?TID_TODAY??";
			$strTagPrefix = "?";
			$strTagPostfix = "?";
			$strSearch = "\n";
			$strReplace = "<br/>";
			$strExpectedResult =
"Loram ipsim blubb<br/>and a new line.<br/>
Hallo World! Hello World<br/>
Plusquam dolom.<br/>
Hello John, how are you today?";
		
			$this->TestCase_ReplaceTagsWithReplaceString(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strSearch,
				$strReplace,
				$strExpectedResult);

/*
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?";
			$strTagPostfix = "?";
			$strSearch = "\n";
			$strReplace = "<br/>";
			$strExpectedResult = "";
			
			$nAmount = 500;
			for ($nIndex = 0; $nIndex <= $nAmount; $nIndex++)
			{
				$strValue = "Line1\nValue=".($nAmount-$nIndex);
				$strTemplate .= "Loram Ipsum ?TID_TEST_".$nIndex."?<br/>";
				$strExpectedResult .= "Loram Ipsum Line1<br/>Value=".$nIndex."<br/>";
				$arrayTags["TID_TEST_".$nIndex] = "Line1<br/>Value=".$nIndex;
			}


			$this->TestCase_ReplaceTagsWithReplaceString(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strSearch,
				$strReplace,
				$strExpectedResult);
*/
	
	
	
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?";
			$strTagPostfix = "?";
			$strSearch = "\n";
			$strReplace = "<br/>";
			$strExpectedResult = "";
			
			$nAmount = 500; 
			for ($nIndex = 0; $nIndex <= $nAmount; $nIndex++)
			{
				$strValue = u("Line1 ÄÖÜ\nValue=".($nAmount-$nIndex));
				$strTemplate .= u("Loram Ipsum ?TID_TEST_".$nIndex."?<br/>");
				$strExpectedResult .= u("Loram Ipsum Line1 ÄÖÜ<br/>Value=".$nIndex."<br/>");
				$arrayTags["TID_TEST_".$nIndex] = u("Line1 ÄÖÜ<br/>Value=".$nIndex);
			}


			$this->TestCase_ReplaceTagsWithReplaceString(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strSearch,
				$strReplace,
				$strExpectedResult);
	
	
		}
		

	}
	
	

		
