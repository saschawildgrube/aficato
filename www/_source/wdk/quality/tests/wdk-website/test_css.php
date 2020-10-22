<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Style Sheet Inclusion");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-css";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'color: #123456;'
					)
				);
		}
		

		
	}
	
	
		


		
