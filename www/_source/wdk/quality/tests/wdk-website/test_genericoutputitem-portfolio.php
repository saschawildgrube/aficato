<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Generic Output Item: Portfolio");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItem($strContent,$arrayExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=".$strContent,
				$arrayExpectedOutput);
			
			$this->Trace("");	
			$this->Trace("");	
			
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);
			
			$this->TestCase_GenericOutputItem(
				"test-genericoutputitem-portfolio",
				array('
<div>
<div><div>
	<h2>Title One</h2>
	<img src="http://'.GetRootURL().'quality/testwebsite/images/test.png" alt=""/>
	<p>This is text number one.</p>
	<div><a href="http://www.example.com" target="_blank">Link One</a>
</div>
</div></div><div><div>
	<h2>Title Two</h2>
	<img src="http://'.GetRootURL().'quality/testwebsite/images/test.png" alt=""/>
	<p>This is text number two.</p>
	<div><a href="http://www.example.com">Link Two</a>
</div>
</div></div>
</div>')
			);

				
				
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
