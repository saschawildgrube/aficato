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
					'GetDeviceType()    : mobile',
					'IsDeviceMobile()   : true',
					'IsDeviceTablet()   : false',
					'IsDeviceComputer() : false',
					'IsDeviceWatch()    : false',
					'IsDeviceTV()       : false',),
				array(),
				array(),
				array(),
				array(), 
				array(
					'User-Agent' => 'Mozilla/5.0 (iPhone; CPU iPhone OS 10_2_1 like Mac OS X) AppleWebKit/602.4.6 (KHTML, like Gecko) Version/10.0 Mobile/14D27 Safari/602.1'));

			$strOutput = $this->TestCase_CheckURL(
				$strURL,
				array(
					'GetDeviceType()    : mobile',
					'IsDeviceMobile()   : true',
					'IsDeviceTablet()   : false',
					'IsDeviceComputer() : false',
					'IsDeviceWatch()    : false',
					'IsDeviceTV()       : false',),
				array(),
				array(),
				array(),
				array(), 
				array(
					'User-Agent' => 'Mozilla/5.0 (Linux; U; Android 2.2.1; en-us; Nexus One Build/FRG83) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1'));

		}
		

		
	}
	
	
		


		
