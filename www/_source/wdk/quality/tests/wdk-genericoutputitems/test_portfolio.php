<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test generic output item PORTFOLIO");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItem($strContent,$strExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=test-genericoutputitem-".$strContent,
				array($strExpectedOutput));			
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);

			$strExpected =
'<div>
	<div><div>
		<h2>Title One</h2>
		<img src="http://'.GetRootURL().'quality/testwebsite/images/test.png" alt=""/>
		<p>This is text number one.</p>
		<div><a href="http://www.example.com" target="_blank">Link One</a></div>
	</div></div>
	<div><div>
		<h2>Title Two</h2>
		<img src="http://'.GetRootURL().'quality/testwebsite/images/test.png" alt=""/>
		<p>This is text number two.</p>
		<div><a href="http://www.example.com">Link Two</a></div>
	</div></div>
</div>';

			$this->TestCase_GenericOutputItem(
				"portfolio",
				$strExpected);
			

		}
		

		 
		
	} 
	
	
		


		
