<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Icon");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-icon";

			$this->TestCase_CheckURL(
				$strURL,
				array('<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_link" alt="" title="An icon without a link" style="vertical-align:middle;"/>'));
		}
		

		
	}
	
	
		


		
