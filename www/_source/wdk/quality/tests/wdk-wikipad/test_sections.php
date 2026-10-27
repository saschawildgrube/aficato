<?php

	require_once(GetWDKDir().'wdk_websitesatellite.inc');
	require_once(GetWDKDir().'wdk_module.inc');
	require_once(GetWDKDir().'modules/cms/wikipad/module_wikipad.inc');

	class CWikiPadSectionRangeProbe extends CWikiPadModule
	{
		// Range parsing is independent of website state and storage.
		function __construct() {}
	}

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WikiPad heading and separator sections');
		}

		function CheckSections($strSource,$arrayExpected)
		{
			$module = new CWikiPadSectionRangeProbe();
			$arraySections = $module->GetEditableSections($strSource);
			$arrayActual = array();
			foreach ($arraySections as $section)
			{
				$arrayActual[] = array($section['type'],substr($strSource,$section['start'],$section['length']));
			}
			if ($arrayActual !== $arrayExpected)
			{
				$this->Trace(array('source' => $strSource,'expected' => $arrayExpected,'actual' => $arrayActual));
				$this->SetResult(false);
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckSections('',array());
			$this->CheckSections("Introduction\n<h1>Raw HTML</h1>\n<hr/>\n=Unclosed\n---",array());
			$this->CheckSections("Intro\n=Parent=\nparent\n==Child==\nchild\n----\nafter rule\n===Deep===\nlast",array(
				array('heading',"=Parent=\nparent\n"),
				array('heading',"==Child==\nchild\n"),
				array('separator',"----\nafter rule\n"),
				array('heading',"===Deep===\nlast")));
			$this->CheckSections("=Same=\n==Same==\n===Same===\n====Same====\n=====Same=====\n======Same======",array(
				array('heading',"=Same=\n"),array('heading',"==Same==\n"),
				array('heading',"===Same===\n"),array('heading',"====Same====\n"),
				array('heading',"=====Same=====\n"),array('heading','======Same======')));
			$this->CheckSections("----\n-----\n----",array(
				array('separator',"----\n"),array('separator',"-----\n"),array('separator','----')));
			$this->CheckSections("==Duplicate==\nfirst\n==Duplicate==\nlast",array(
				array('heading',"==Duplicate==\nfirst\n"),array('heading',"==Duplicate==\nlast")));
			$this->CheckSections("Intro\r\n  ==One== \r\ntext\r\n\t---- \r\nend",array(
				array('heading',"  ==One== \r\ntext\r\n"),array('separator',"\t---- \r\nend")));
			$strLiteral = "=Real=\n<pre>\n==Fake==\n----\n</pre>\n".
				"<SYNTAXHIGHLIGHT lang=\"text\">\n<pre>\n=Also fake=\n----\n</SYNTAXHIGHLIGHT>\n";
			$this->CheckSections($strLiteral."==Next==\nend",array(
				array('heading',$strLiteral),array('heading',"==Next==\nend")));
			$this->CheckSections("=Real=\n<pre>\n==Fake==\n----",array(
				array('heading',"=Real=\n<pre>\n==Fake==\n----")));
			$this->CheckSections("Intro \xc3\xa4\n=\xc3\xa9=\n\xe2\x82\xac\n----\nlast",array(
				array('heading',"=\xc3\xa9=\n\xe2\x82\xac\n"),array('separator',"----\nlast")));
		}
	}
