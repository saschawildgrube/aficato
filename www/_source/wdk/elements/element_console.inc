<?php

	class CConsoleElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'console',
				$strStyleID,
				false,
				true);
		}
		
		function RenderConsole($strConsole)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$strConsole = ReplaceString($strConsole,"\r",''); 
			//print($strConsole);
			$strConsole = ReplaceString($strConsole,' ','&nbsp;');
			$strConsole = HtmlDecode($strConsole);
			$strConsole = StringWordWrap($strConsole,75,"\n",true);
			$strConsole = HtmlEncode($strConsole);
			$strConsole = ReplaceString($strConsole,"\n",'<br/>');
			//$strConsole = ReplaceString($strConsole,' ','&nbsp;');
			$strConsole = ReplaceString($strConsole,"\t",'&nbsp;&nbsp;&nbsp;');
			$arrayItems['CONSOLE'] = $strConsole;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

