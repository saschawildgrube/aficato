<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReplaceTags");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ReplaceTags(
			$strTemplate,
			$arrayTags,
			$strTagPrefix,
			$strTagPostfix,
			$strExpectedResult,
			$strComment = "")
		{
			$this->Trace("TestCase_ReplaceTags: ".$strComment);
			$this->Trace("StringLength(\$strTemplate): ".StringLength($strTemplate));
			$this->Trace("ArrayCount(\$arrayTags)_________: ".ArrayCount($arrayTags));
			$this->Trace("Tag prefixes in template__: ".StringCount($strTemplate,$strTagPrefix));
			
			$sw = new CStopWatch();
			$sw->Start();
			$strResult = ReplaceTags(  
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix);
			$sw->Stop();
			
		
			$this->Trace("Tag prefixes in result____: ".StringCount($strResult,$strTagPrefix));
			if ($strResult != $strExpectedResult)
			{
				$this->Trace("Testcase FAILED for ReplaceTags()");	
				$this->SetResult(false);
			}
			else
			{
				$this->Trace("Testcase PASSED!");
			}
				
				/*
				$this->Trace("strTemplate:");
				$this->Trace($strTemplate);
				$this->Trace("Expected Result:");
				$this->Trace($strExpectedResult);
				$this->Trace("ReplaceTags returns:");
				$this->Trace($strResult);
				*/
				
			$this->Trace("Elapsed time (secs) ReplaceTags() : ".$sw->GetSeconds());
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
"Loram ipsim ?TID_ITEM1?\n
Hallo ?TID_WORLD?! Hello ?TID_WORLD?\n
Plusquam dolom.\n
Hello ?TID_NAME?, how are you ?TID_TODAY??";
			$strTagPrefix = "?";
			$strTagPostfix = "?";
			$strExpectedResult =
"Loram ipsim blubb\nand a new line.\n
Hallo World! Hello World\n
Plusquam dolom.\n
Hello John, how are you today?";
		
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpectedResult);


			$strBogus = "";
			for  ($nIndex = 0; $nIndex < 50; $nIndex++)   
			{
				$strBogus .= "1234567890";	
			}


	
	
	
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?TID_";
			$strTagPostfix = "?";
			$strExpectedResult = "";
			$nAmount = 500; 
			for ($nIndex = 1; $nIndex <= $nAmount; $nIndex++)
			{
				$strTemplate.=$strBogus;
				$strExpectedResult.=$strBogus;
				
				$strValue = u("Line1 ÄÖÜ\nValue=".($nAmount-$nIndex));
				$strTemplate .= u("Loram Ipsum ?TID_TEST_".$nIndex."?\n");
				$strExpectedResult .= u("Loram Ipsum Line1 ÄÖÜ\nValue=".$nIndex."\n");
				$arrayTags["TEST_".$nIndex] = u("Line1 ÄÖÜ\nValue=".$nIndex);
			}
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpectedResult,
				"Template and array contain MANY tags.");
				
				
				
		
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?TID_";
			$strTagPostfix = "?";
			$strExpectedResult = "";
			$nAmount = 500; 
			//$strBogus = "70239847320498237048256465456456546464645646546464564564564565464456465664564564564654646456464564564654drtervtesrertestvivjrisdjölkjdövlksöglskdjörejtösirjstvörjstölvkdjöl73049287304982374029387402938740239847203984720394872039487209348720348720349827304987230498273049827340982374029387402387402398472\n";
			for ($nIndex = 1; $nIndex <= $nAmount; $nIndex++)
			{
				$strTemplate.=$strBogus;
				$strExpectedResult.=$strBogus;
				$strValue = u("Line1 ÄÖÜ\nValue=".($nAmount-$nIndex));
				if ($nIndex == 100)
				{
					$strTemplate .= u("Loram Ipsum ?TID_TEST_".$nIndex."?\n");
					$strExpectedResult .= u("Loram Ipsum Line1 ÄÖÜ\nValue=".$nIndex."\n");
				}
				else
				{
					$strTemplate .= u("Loram Ipsum BLUBBER\n");
					$strExpectedResult .= u("Loram Ipsum BLUBBER\n");
				}
				$arrayTags["TEST_".$nIndex] = u("Line1 ÄÖÜ\nValue=".$nIndex);
			}
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpectedResult,
				"Template has 1, array has MANY tags");				
	
	
	
	
	
	
			
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?TID_";
			$strTagPostfix = "?";
			$strExpectedResult = ""; 
			$nAmount = 500; 
			//$strBogus = "70239847320498237048256465456456546464645646546464564564564565464456465664564564564654646456464564564654drtervtesrertestvivjrisdjölkjdövlksöglskdjörejtösirjstvörjstölvkdjöl73049287304982374029387402938740239847203984720394872039487209348720348720349827304987230498273049827340982374029387402387402398472\n";
			for ($nIndex = 1; $nIndex <= $nAmount; $nIndex++)
			{
				$strTemplate.=$strBogus;
				$strExpectedResult.=$strBogus;
				$strValue = u("Line1 ÄÖÜ\nValue=".($nAmount-$nIndex));
				$strTemplate .= u("Loram Ipsum ?TID_TEST_1?\n");
				$strExpectedResult .= u("Loram Ipsum Line1 ÄÖÜ\nValue=1\n");
				$arrayTags["TEST_".$nIndex] = u("Line1 ÄÖÜ\nValue=".$nIndex);
			}
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpectedResult,
				"Template has MANY times the SAME tag. Array has many.");				
	
	
	
			$strTemplate = "";
			$arrayTags = array();
			$strTagPrefix = "?TID_";
			$strTagPostfix = "?";
			$strExpectedResult = ""; 
			$nAmount = 500; 
			//$strBogus = "70239847320498237048256465456456546464645646546464564564564565464456465664564564564654646456464564564654drtervtesrertestvivjrisdjölkjdövlksöglskdjörejtösirjstvörjstölvkdjöl73049287304982374029387402938740239847203984720394872039487209348720348720349827304987230498273049827340982374029387402387402398472\n";
			for ($nIndex = 1; $nIndex <= $nAmount; $nIndex++)
			{
				$strTemplate.=$strBogus;
				$strExpectedResult.=$strBogus;
				$strValue = u("Line1 ÄÖÜ\nValue=".($nAmount-$nIndex));
				$strTemplate .= u("Loram Ipsum ?TID_TEST_1?\n");
				$strExpectedResult .= u("Loram Ipsum Line1 ÄÖÜ\nValue=1\n");
			}
			$arrayTags["TEST_1"] = u("Line1 ÄÖÜ\nValue=1");
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpectedResult,
				"Template has MANY times the SAME tag. Array has 1.");				
	
	
	
		}
		

	}
	
	

		
