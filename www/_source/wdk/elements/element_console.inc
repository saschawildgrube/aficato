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
				true,
				true);
		}
		
		function RenderConsole($strConsole)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$nLineMaxWidth = GetIntegerValue($this->GetLayoutDef('LAYOUT_LINE_MAX_WIDTH'));
			if ($nLineMaxWidth == 0)
			{
				$nLineMaxWidth = 100;
			}
					
			$strConsole = ReplaceString($strConsole,"\r",''); 
			$strConsole = ReplaceString($strConsole,' ','&nbsp;');
			$strConsole = HtmlDecode($strConsole);
			$strConsole = StringWordWrap($strConsole,$nLineMaxWidth,"\n",true);
			$strConsole = HtmlEncode($strConsole);
			$strConsole = ReplaceString($strConsole,"\n",'<br/>');
			$strConsole = ReplaceString($strConsole,"\t",'&nbsp;&nbsp;&nbsp;');
			$arrayItems['CONSOLE'] = $strConsole;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

