<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Image Inclusion");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			$this->SetResult(true);
			
			//$this->SetActive(false);
			
			return true;
		}
		
		function TestCase_Image(
			$strTestCase,
			$strImageFile,
			$strImageURL)
		{
			$this->Trace($strTestCase);
			$strTestImage = FileReadBinary($strImageFile);
			$this->TestCase_CheckURL(
				$strImageURL,
				array($strTestImage),
				array(),array(),array(),array(),array("Accept-Encoding" => "identity"),"get",15,true,
				true);
			$this->Trace("");				
		}
		
		function OnTest()
		{
			parent::OnTest();

			$this->Trace("");
			$this->Trace("PART 1: Testing the image URL generator");
			$this->Trace("");
			
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-images";
			$this->TestCase_CheckURL(
				$strURL,
				array(
					'<p>ROOT-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/images/test-root-png.png"/></p>',
					'<p>ROOT-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/images/test-root-jpg.jpg"/></p>',
					'<p>ROOT-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/images/test-root-jpg.jpg"/></p>',
					'<p>ROOT-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/images/test-root-gif.png"/></p>',
					'<p>ROOT-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/images/test-root-gif.gif"/></p>',
					'<p>SOURCE-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-source-png"/></p>',
					'<p>SOURCE-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-source-jpg"/></p>',
					'<p>SOURCE-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-source-jpg&amp;ext=jpg"/></p>',
					'<p>SOURCE-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-source-gif"/></p>',
					'<p>SOURCE-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-source-gif&amp;ext=gif"/></p>',
					'<p>ASSEMBLY-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-assembly-png"/></p>',
					'<p>ASSEMBLY-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-assembly-jpg"/></p>',
					'<p>ASSEMBLY-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-assembly-jpg&amp;ext=jpg"/></p>',
					'<p>ASSEMBLY-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-assembly-gif"/></p>',
					'<p>ASSEMBLY-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-assembly-gif&amp;ext=gif"/></p>',
					'<p>LAYOUT-ROOT-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/images/layout_default_test-layout-root-png.png"/></p>',
					'<p>LAYOUT-ROOT-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/images/layout_default_test-layout-root-jpg.jpg"/></p>',
					'<p>LAYOUT-ROOT-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/images/layout_default_test-layout-root-jpg.jpg"/></p>',
					'<p>LAYOUT-ROOT-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/images/layout_default_test-layout-root-gif.png"/></p>',
					'<p>LAYOUT-ROOT-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/images/layout_default_test-layout-root-gif.gif"/></p>',
					'<p>LAYOUT-SOURCE-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-source-png"/></p>',
					'<p>LAYOUT-SOURCE-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-source-jpg"/></p>',
					'<p>LAYOUT-SOURCE-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-source-jpg&amp;ext=jpg"/></p>',
					'<p>LAYOUT-SOURCE-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-source-gif"/></p>',
					'<p>LAYOUT-SOURCE-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-source-gif&amp;ext=gif"/></p>',
					'<p>LAYOUT-ASSEMBLY-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-assembly-png"/></p>',
					'<p>LAYOUT-ASSEMBLY-JPG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-assembly-jpg"/></p>',
					'<p>LAYOUT-ASSEMBLY-JPG-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-assembly-jpg&amp;ext=jpg"/></p>',
					'<p>LAYOUT-ASSEMBLY-GIF:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-assembly-gif"/></p>',
					'<p>LAYOUT-ASSEMBLY-GIF-EXT:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-layout-assembly-gif&amp;ext=gif"/></p>',
					'<p>MIXED-SOURCE-PNG:<img src="http://'.GetRootURL().'quality/testwebsite/?layout=default&amp;command=image&amp;id=test-mixed-source-png"/></p>',
					'<p>'
					));
					
			$this->Trace("");
			$this->Trace("PART 2: Testing the image URL resolver within CWebsite");

			$this->Trace("");
			$this->Trace("PART 2.1 - Non-layout images");
			$this->Trace("");

			$this->TestCase_Image(
				"ROOT-PNG",
				GetDocumentRootDir()."quality/testwebsite/images/test-root-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-root-png");

			$this->TestCase_Image(
				"ROOT-JPG",
				GetDocumentRootDir()."quality/testwebsite/images/test-root-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-root-jpg");

			$this->TestCase_Image(
				"ROOT-JPG-EXT",
				GetDocumentRootDir()."quality/testwebsite/images/test-root-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-root-jpg&ext=jpg");

			$this->TestCase_Image(
				"ROOT-GIF (Should return the png since the extention is withheld)",
				GetDocumentRootDir()."quality/testwebsite/images/test-root-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-root-gif");

			$this->TestCase_Image(
				"ROOT-GIF-EXT: (Should return the gif since the extention is given)",
				GetDocumentRootDir()."quality/testwebsite/images/test-root-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-root-gif&ext=gif");


			
			$this->TestCase_Image(
				"SOURCE-PNG",
				GetWDKDir()."quality/testwebsite/_source/images/test-source-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-source-png");

			$this->TestCase_Image(
				"SOURCE-JPG",
				GetWDKDir()."quality/testwebsite/_source/images/test-source-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-source-jpg");

			$this->TestCase_Image(
				"SOURCE-JPG-EXT",
				GetWDKDir()."quality/testwebsite/_source/images/test-source-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-source-jpg&ext=jpg");

			$this->TestCase_Image(
				"SOURCE-GIF (Should return the png since the extention is withheld)",
				GetWDKDir()."quality/testwebsite/_source/images/test-source-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-source-gif");

			$this->TestCase_Image(
				"SOURCE-GIF-EXT: (Should return the gif since the extention is given)",
				GetWDKDir()."quality/testwebsite/_source/images/test-source-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-source-gif&ext=gif");


			$this->TestCase_Image(
				"ASSEMBLY-PNG",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/test-assembly-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-assembly-png");

			$this->TestCase_Image(
				"ASSEMBLY-JPG",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/test-assembly-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-assembly-jpg");

			$this->TestCase_Image(
				"ASSEMBLY-JPG-EXT",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/test-assembly-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-assembly-jpg&ext=jpg");

			$this->TestCase_Image(
				"ASSEMBLY-GIF",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/test-assembly-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-assembly-gif");

			$this->TestCase_Image(
				"ASSEMBLY-GIF-EXT",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/test-assembly-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-assembly-gif&ext=gif");


			$this->Trace("");
			$this->Trace("PART 2.2 - Layout images");
			$this->Trace("");

			$this->TestCase_Image(
				"LAYOUT-ROOT-PNG",
				GetDocumentRootDir()."quality/testwebsite/images/layout_default_test-layout-root-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-root-png");

			$this->TestCase_Image(
				"LAYOUT-ROOT-JPG",
				GetDocumentRootDir()."quality/testwebsite/images/layout_default_test-layout-root-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-root-jpg");

			$this->TestCase_Image(
				"LAYOUT-ROOT-JPG-EXT",
				GetDocumentRootDir()."quality/testwebsite/images/layout_default_test-layout-root-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-root-jpg&ext=jpg");

			$this->TestCase_Image(
				"LAYOUT-ROOT-GIF (Should return the png since the extention is withheld)",
				GetDocumentRootDir()."quality/testwebsite/images/layout_default_test-layout-root-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-root-gif");

			$this->TestCase_Image(
				"LAYOUT-ROOT-GIF-EXT: (Should return the gif since the extention is given)",
				GetDocumentRootDir()."quality/testwebsite/images/layout_default_test-layout-root-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-root-gif&ext=gif");


			
			$this->TestCase_Image(
				"LAYOUT-SOURCE-PNG",
				GetWDKDir()."quality/testwebsite/_source/images/layout_default_test-layout-source-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-source-png");

			$this->TestCase_Image(
				"LAYOUT-SOURCE-JPG",
				GetWDKDir()."quality/testwebsite/_source/images/layout_default_test-layout-source-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-source-jpg");

			$this->TestCase_Image(
				"LAYOUT-SOURCE-JPG-EXT",
				GetWDKDir()."quality/testwebsite/_source/images/layout_default_test-layout-source-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-source-jpg&ext=jpg");

			$this->TestCase_Image(
				"LAYOUT-SOURCE-GIF (Should return the png since the extention is withheld)",
				GetWDKDir()."quality/testwebsite/_source/images/layout_default_test-layout-source-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-source-gif");

			$this->TestCase_Image(
				"LAYOUT-SOURCE-GIF-EXT: (Should return the gif since the extention is given)",
				GetWDKDir()."quality/testwebsite/_source/images/layout_default_test-layout-source-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-source-gif&ext=gif");


			$this->TestCase_Image(
				"LAYOUT-ASSEMBLY-PNG",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/layout_default_test-layout-assembly-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-assembly-png");

			$this->TestCase_Image(
				"LAYOUT-ASSEMBLY-JPG",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/layout_default_test-layout-assembly-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-assembly-jpg");

			$this->TestCase_Image(
				"LAYOUT-ASSEMBLY-JPG-EXT",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/layout_default_test-layout-assembly-jpg.jpg",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-assembly-jpg&ext=jpg");

			$this->TestCase_Image(
				"LAYOUT-ASSEMBLY-GIF",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/layout_default_test-layout-assembly-gif.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-assembly-gif");

			$this->TestCase_Image(
				"LAYOUT-ASSEMBLY-GIF-EXT",
				GetWDKDir()."quality/testwebsite/_source/assemblies/testimages/images/layout_default_test-layout-assembly-gif.gif",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-layout-assembly-gif&ext=gif");


			$this->Trace("");
			$this->Trace("PART 2.3 - Mixed cases");
			$this->Trace("");

			$this->TestCase_Image(
				"MIXED-SOURCE-PNG",
				GetWDKDir()."quality/testwebsite/_source/images/test-mixed-source-png.png",
				"http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test-mixed-source-png");


		}
		

		
	}
	
	
		


		
