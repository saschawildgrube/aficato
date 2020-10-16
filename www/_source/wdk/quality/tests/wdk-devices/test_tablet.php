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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-devices";

			$strOutput = $this->TestCase_CheckURL(
				$strURL,
				array(
					'GetDeviceType()    : tablet',
					'IsDeviceMobile()   : false',
					'IsDeviceTablet()   : true',
					'IsDeviceComputer() : false',
					'IsDeviceWatch()    : false',
					'IsDeviceTV()       : false',),
				array(),
				array(),
				array(),
				array(), 
				array(
					'User-Agent' => 'Mozilla/5.0 (iPad; U; CPU OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5'));

		}
		

		
	}
	
	
		


		
