<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Link");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-link";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<a href="http://www.example.com">http://www.example.com</a>',
					'<a href="http://www.example.com">Example</a>',
					'<a href="http://www.example.com" target="_blank">Example (external)</a>'
					)
				);
		}
		

		
	}
	
	
		


		
