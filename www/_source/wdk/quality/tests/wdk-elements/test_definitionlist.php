<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Definition List");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			return true;
		}
		
		function TestCase_DefinitionListElement($strContent,$strExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-".$strContent,
				array($strExpectedOutput));			
		}
		
		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-definitionlist";
			$this->Trace("URL: ".$strURL);

			$strExpected = "<dl>
<dt>Alpha</dt>
<dd>This first letter in the Greek alphabet</dd>
<dt>Zulu</dt>
<dd>Represents the letter Z in the NATO alphabet</dd>
<dd>An African tribe</dd>
</dl>";

			$this->TestCase_DefinitionListElement(
				"definitionlist1",
				$strExpected);
			

		}
		

		 
		
	} 
	
	
		


		
