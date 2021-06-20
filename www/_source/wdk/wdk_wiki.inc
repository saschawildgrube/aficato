<?php
	
	function ParseWiki($strWiki)
	{
		$arrayWiki = [];
		
		$arrayLines = StringExplode($strWiki,"\n",true);
		
		$arrayWikiLines = [];
		$nLine = 0;
		foreach ($arrayLines as $strLine)
		{
			$arrayWikiLine = array();
			$arrayWikiLine['number'] = $nLine;
			$arrayWikiLine['raw'] = $strLine;

			//$strLine = StringTrim($strLine);

			$arrayWikiLines[] = $arrayWikiLine;
			$nLine++;
		}
		
		$arrayWiki['lines'] = $arrayWikiLines;
		
		return $arrayWiki;
	}



	

