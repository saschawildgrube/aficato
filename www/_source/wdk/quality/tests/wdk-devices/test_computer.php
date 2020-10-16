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
					'GetDeviceType()    : computer',
					'IsDeviceMobile()   : false',
					'IsDeviceTablet()   : false',
					'IsDeviceComputer() : true',
					'IsDeviceWatch()    : false',
					'IsDeviceTV()       : false',),
				array(),
				array(),
				array(),
				array(), 
				array(
					'User-Agent' => 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; Trident/7.0; rv:11.0) like Gecko'));
					
		
		}
		

		
	}
	
	
		


		
