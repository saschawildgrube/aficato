<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Download");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);
			
//			$this->SetActive(false);
//			$this->Trace("The \"download from assembly\" feature is not yet ready.");
			
			return true;
		}
		
		function TestCase_Download(
			$strTestCase,
			$strDownloadFile,
			$strDownloadURL)
		{
			$this->Trace($strTestCase);
			$strTestFile = FileReadBinary($strDownloadFile);
			$this->TestCase_CheckURL(
				$strDownloadURL,
				array($strTestFile),
				array(),array(),array(),array(),array("Accept-Encoding" => "identity"),"get",15,true,
				true);
			$this->Trace("");				
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$this->Trace("");
			$this->Trace("PART 1: Testing the download URL generator");
			$this->Trace("");
			
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-download";
			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<p><a href="http://'.GetRootURL().'quality/testwebsite/?command=download&amp;filepath=test-assembly.pdf">DOWNLOAD FROM ASSEMBLY</a></p>',
					'<p>'
					));

			$this->Trace("");
			$this->Trace("PART 2: Testing the file URL resolver within CWebsite");
			$this->Trace("");
			
			$this->TestCase_Download(
				"DOWNLOAD FROM ASSEMBLY",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testdownload/download/test-assembly.pdf",
				"http://".GetRootURL()."quality/testwebsite/?command=download&filepath=test-assembly.pdf");

		}
		

		
	}
	
	
		


		
