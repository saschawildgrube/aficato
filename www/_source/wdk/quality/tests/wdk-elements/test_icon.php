<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test Element Icon');
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

			$strURL = 'http://'.GetRootURL().'quality/testwebsite/?content=test-element-icon';

			$this->TestCase_CheckURL(
				$strURL,
				array('<i class="fa fa-link fa-fw" aria-hidden="true"></i>'));
		}
		

		
	}
	
	
		


		
