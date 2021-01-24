<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test generic output item LINK');
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItem($strContent,$strExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = 'http://'.GetRootURL().'quality/testwebsite/?content=test-genericoutputitem-'.$strContent,
				array($strExpectedOutput));			
		}
		
		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);

			$strExpected =
'<a href="http://www.example.com">Go to a website</a>
<br/>
<a href="http://www.example.com">http://www.example.com</a>
<br/>
<a href="http://www.example.com" target="_blank">http://www.example.com</a>
<br/>
<a href="mailto:mail@example.com">Send a Mail</a>
<br/>
<a href="mailto:mail@example.com">mail@example.com</a>';

			$this->TestCase_GenericOutputItem(
				"link",
				$strExpected);
			

		}
		

		 
		
	} 
	
	
		


		
