<?php

	require_once(GetWDKDir().'wdk_websitesatellite.inc');
	require_once(GetWDKDir().'modules/cms/staticfileviewer/module_staticfileviewer.inc');

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Static file viewer classification, containment and literal text');
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			foreach (array('readme.txt','script.BAT','source.php','source.inc','app.tsx','data.json','.htaccess') as $strFile)
			{
				if (!CStaticFileViewerModule::IsTextFile($strFile))
				{
					$this->SetResult(false);
				}
			}
			foreach (array('image.png','archive.zip','app.exe','file.pdf','file.txt.png','unknown') as $strFile)
			{
				if (CStaticFileViewerModule::IsTextFile($strFile))
				{
					$this->SetResult(false);
				}
			}
			$strRoot = dirname(__FILE__);
			if (CStaticFileViewerModule::ResolveFile($strRoot,basename(__FILE__)) !== realpath(__FILE__) ||
				CStaticFileViewerModule::ResolveFile($strRoot,'../wdk-wikipad/test_sections.php') !== false ||
				CStaticFileViewerModule::ResolveFile($strRoot,'missing.txt') !== false ||
				CStaticFileViewerModule::ResolveFile($strRoot,"bad\0.txt") !== false ||
				CStaticFileViewerModule::ResolveFile($strRoot,'.') !== false)
			{
				$this->SetResult(false);
			}
			$strSource = "<script>alert('test')</script>\n<?php {TOOLBAR} ?TID_TITLE? & text";
			$strEncoded = CStaticFileViewerModule::EncodeText($strSource);
			if (strpos($strEncoded,'<') !== false || strpos($strEncoded,'{') !== false ||
				strpos($strEncoded,'?') !== false || HtmlDecode($strEncoded) !== $strSource)
			{
				$this->SetResult(false);
			}
		}
	}
