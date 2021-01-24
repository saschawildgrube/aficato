<?php
	
	class CTest extends CUnitTest
	{
		var $m_arrayProfiling;
		
		function __construct()
		{
			parent::__construct("ReplaceTags Method Comparison");
			$this->m_arrayProfiling = array();
		}
		
		function OnInit()
		{
			parent::OnInit();
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
			$this->Trace('TestCase_ReplaceTags____________________: '.$strComment);
			$this->Trace('$strTagPrefix___________________________: "'.$strTagPrefix.'"');
			$this->Trace('$strTagPostfix__________________________: "'.$strTagPostfix.'"');
			$this->Trace('StringLength($strTemplate)______________: '.StringLength($strTemplate));
			$this->Trace('StringCount($strTemplate,$strTagPrefix)_: '.StringCount($strTemplate,$strTagPrefix));
			$this->Trace('ArrayCount($arrayTags)__________________: '.ArrayCount($arrayTags));
			


			$sw_rt = new CStopWatch();
			$sw_rt->Start();
			$strResult = ReplaceTags(  
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix);
			$sw_rt->Stop();
			
			$sw_rt_tl = new CStopWatch();
			$sw_rt_tl->Start();
			$strResultTL = ReplaceTags_TagLoop(  
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix);
			$sw_rt_tl->Stop();
			
			$sw_rt_snt = new CStopWatch();
			$sw_rt_snt->Start();
			$strResultSNT = ReplaceTags_SearchNextTag(  
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix); 
			$sw_rt_snt->Stop();
			
			
			
			$bCaseResult = true;
			if ($strResult != $strExpectedResult)
			{
				$this->Trace("Testcase FAILED for ReplaceTags()");	
				$bCaseResult = false;
				$this->SetResult(false);
			}
			if ($strResultTL != $strExpectedResult)
			{
				$this->Trace("Testcase FAILED for ReplaceTags_TagLoop()");	
				$bCaseResult = false;
				$this->SetResult(false);
			}
			if ($strResultSNT != $strExpectedResult)
			{
				$this->Trace("Testcase FAILED for ReplaceTags_SearchNextTag()");
				$bCaseResult = false;
				$this->SetResult(false);
			}
			if ($bCaseResult == true)
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
			
			$this->Trace('ReplaceTags()______________: '.RenderNumber($sw_rt->GetSeconds(),4).' secs');	
			$this->Trace('ReplaceTags_TagLoop()______: '.RenderNumber($sw_rt_tl->GetSeconds(),4).' secs');
			$this->Trace('ReplaceTags_SearchNextTag(): '.RenderNumber($sw_rt_snt->GetSeconds(),4).' secs');
			
			$this->m_arrayProfiling['ReplaceTags'][] = $sw_rt->GetSeconds();
			$this->m_arrayProfiling['ReplaceTags_TagLoop'][] = $sw_rt_tl->GetSeconds();
			$this->m_arrayProfiling['ReplaceTags_SearchNextTag'][] = $sw_rt_snt->GetSeconds();
			
			$this->Trace("");
			
		}

	
		function MakeTags($nCount,$nValuePostfixLength = 0)
		{
			$strValuePostfix = StringRepeat('a',$nValuePostfixLength);
			$arrayTags = array();
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$arrayTags['TAG_'.$nIndex] = 'Value='.$nIndex.$strValuePostfix;
			}
			return $arrayTags;	
		}

		function MakeTemplateAndExpected($arrayTags,$strTagPrefix,$strTagPostfix,$nTagCount,$nDistinctTags,$nInterimTextLength,&$strTemplate,&$strExpected)
		{
			$strTemplate = '';
			$strExpected = '';
			$strInterimText = '';
			
			for ($nCounter = 0; $nCounter < $nInterimTextLength; $nCounter++)
			{
				$strInterimText .= 'a';
			}
			//$strInterimText = StringRepeat();
			
			$strTemplate .= $strInterimText;
			$strExpected .= $strInterimText;
			
			$arrayTagKeys = array_keys($arrayTags);
			
			$nTagIndex = 0;
			for ($nCounter = 0; $nCounter < $nTagCount; $nCounter++)
			{
				$strTemplate .= $strTagPrefix.$arrayTagKeys[$nTagIndex].$strTagPostfix;
				$strExpected .= $arrayTags[$arrayTagKeys[$nTagIndex]];
				$nTagIndex++;
				if ($nTagIndex >= $nDistinctTags)
				{
					$nTagIndex = 0;	
				}
				$strTemplate .= $strInterimText;
				$strExpected .= $strInterimText;
			}
		}


		function TestCaseSet_ReplaceTags($strTagPrefix,$strTagPostfix)
		{
			
			$this->Trace('');
			$this->Trace('TestCaseSet_ReplaceTags: "'.$strTagPrefix.'"/"'.$strTagPostfix.'"');
			$this->Trace('');

		
			$arrayTags = $this->MakeTags(5);
			$strTemplate = "";
			$strExpected = "";
			$this->MakeTemplateAndExpected(
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				5,
				5,
				30,
				$strTemplate,
				$strExpected);
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpected,
				"5 tags, 5 distinct");




			$arrayTags = $this->MakeTags(1000);
			$strTemplate = "";
			$strExpected = "";
			$this->MakeTemplateAndExpected(
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				1000,
				1000,
				50,
				$strTemplate,
				$strExpected);
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpected,
				"1000 tags, 1000 distinct, 1000 in templ");


		
			$arrayTags = $this->MakeTags(1000);
			$strTemplate = "";
			$strExpected = "";
			$this->MakeTemplateAndExpected(
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				1000,
				1,
				50,
				$strTemplate,
				$strExpected);
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpected,
				"1000 tags, 1 distinct, 1000 in templ");			
	
	
			$arrayTags = $this->MakeTags(1);
			$strTemplate = "";
			$strExpected = "";
			$this->MakeTemplateAndExpected(
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				1000,
				1,
				50,
				$strTemplate,
				$strExpected);
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpected,
				"1 tag, 1 distinct, 1000 in templ");		
				
				
			$arrayTags = $this->MakeTags(1000,1000);
			$strTemplate = "";
			$strExpected = "";
			$this->MakeTemplateAndExpected(
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				1000,
				1000,
				50,
				$strTemplate,
				$strExpected);
			$this->TestCase_ReplaceTags(
				$strTemplate,
				$arrayTags,
				$strTagPrefix,
				$strTagPostfix,
				$strExpected,
				"1000 tags, 1000 distinct, 1000 in templ, LONG values");				
	
	
		}
		
		function OnTest()
		{
			parent::OnTest();

			$this->TestCaseSet_ReplaceTags('{','}');
			$this->TestCaseSet_ReplaceTags('?TAG_','?');  
			$this->TestCaseSet_ReplaceTags('//','//');
			
/*
			$this->Trace('Summary:');
			$this->Trace('ReplaceTags()______________: '.RenderNumber($m_arrayProfiling[''],4).' average secs');	
			$this->Trace('ReplaceTags_TagLoop()______: '.RenderNumber($sw_rt_tl->GetSeconds(),4).' secs');
			$this->Trace('ReplaceTags_SearchNextTag(): '.RenderNumber($sw_rt_snt->GetSeconds(),4).' secs');
*/			
		}	
		
		
	}
	
	

		
