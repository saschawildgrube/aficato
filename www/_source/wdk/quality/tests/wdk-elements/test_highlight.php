<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Highlight");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			return true;
		}
		
		function TestCase_ElementHighlight($strContent,$strExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-".$strContent,
				array($strExpectedOutput));			
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->TestCase_ElementHighlight(
				"highlight1",
				"<textarea>highlighted</textarea><p>This is some text with a <strong>highlighted</strong> text with some entities (&#169;&amp;&#160;).</p>");

			$this->TestCase_ElementHighlight( 
				"highlight2",
				"A <strong>high</strong>lighted text with a line<br/>break");
				
			$this->TestCase_ElementHighlight(
				"highlight3",
				"Here are some German umlauts: <strong>&#196;</strong>&#214;&#220;&#228;&#246;&#252;");
								
			$this->TestCase_ElementHighlight(
				"highlight4",
				"Smith <strong>&amp;</strong> Wesson");
				
			$this->TestCase_ElementHighlight(
				"highlight5",
				"<p>Line<br/><strong>B</strong>reak</p>");
				
				
		}
		

		
	}
	
	
		


		
