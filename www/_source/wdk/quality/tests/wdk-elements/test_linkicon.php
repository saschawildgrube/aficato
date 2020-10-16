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
					'<p><a href="http://www.example.com" title=""><i class="fa fa-link fa-fw" aria-hidden="true"></i></a></p>',
					'<p><a href="http://www.example.com" title="Example"><i class="fa fa-link fa-fw" aria-hidden="true"></i></a></p>',
					'<p><a href="http://www.example.com" target="_blank" title="Example (external)"><i class="fa fa-external-link fa-fw" aria-hidden="true"></i></a></p>'
					));
		}
		

		
	}
	
	
		


		
