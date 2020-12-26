<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test Element Flag');
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

			$strURL = 'http://'.GetRootURL().'quality/testwebsite/?content=test-element-flag';

			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<p><img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_country-deu" alt="" title="Germany" style="vertical-align:middle; padding-right: 3px;"/></p>',
					'<p><img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=icon_country-fra" alt="" title="France" style="vertical-align:middle; padding-right: 3px;"/></p>'
					));
		}
		

		
	}
	
	
		


		
