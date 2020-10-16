<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Java Script Inclusion");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-javascript";

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<script src="http://'.GetRootURL().'quality/testwebsite/js/test1.js"></script>',
					'<script>// Java Script Test 2</script>',
					'<script>// Java Script Test 3</script>',
					'<script>// Java Script Test 4</script>',
					'<script>// Java Script Test 5</script>',
					'<script src="http://www.example.com/js/test6.js"></script>',
					)
				);
		}
		

		
	}
	
	
		


		
