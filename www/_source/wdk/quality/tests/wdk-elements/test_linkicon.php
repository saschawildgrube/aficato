<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element LinkIcon");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-linkicon";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<p><a href="http://www.example.com"><img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_link" alt="" title="" style="vertical-align:middle;"/></a></p>',
					'<p><a href="http://www.example.com"><img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_link" alt="" title="Example" style="vertical-align:middle;"/></a></p>',
					'<p><a href="http://www.example.com" target="_blank"><img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_linkexternal" alt="" title="Example (external)" style="vertical-align:middle;"/></a></p>'
					));
		}
		

		
	}
	
	
		


		
