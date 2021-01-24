<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Style Sheet Inclusion");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			$this->SetResult(true);
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-css";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'color: #123456;'
					)
				);
		}
		

		
	}
	
	
		


		
