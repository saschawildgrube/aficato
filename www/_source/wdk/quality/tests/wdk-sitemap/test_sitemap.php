<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Sitemap");
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

			$strURL_Root = "http://".GetRootURL()."quality/testwebsite/";
			
			$strSitemapXML =
'<?xml version="1.0" encoding="UTF-8"?>
<urlset
	xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd"
	>
	<url>
		<loc>'.$strURL_Root.'en/</loc>
		<priority>1.0</priority>
	</url>';

			$strURL = $strURL_Root."sitemap.xml";
			$this->TestCase_CheckURL(
				$strURL,
				array($strSitemapXML));
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		
		
	}
	
	
		


		
