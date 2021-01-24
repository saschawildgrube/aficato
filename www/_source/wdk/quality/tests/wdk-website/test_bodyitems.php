<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Body Items");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-bodyitems";
			$this->TestCase_CheckURL(
				$strURL,
				array('<body>
	<div>This is not a body item!</div>
	<div id="a">This is a body item!</div>
	<div id="b">And another body item!</div>
</body>'));

		}
		

		
	}
	
	
		


		
