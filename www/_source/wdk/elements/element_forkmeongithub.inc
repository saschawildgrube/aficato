<?php

	// Inspired by https://codepo8.github.io/css-fork-on-github-ribbon/

	class CForkMeOnGithubElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'forkmeongithub',
				$strStyleID,
				true,
				true);
		}
		
		function RenderForkMeOnGithub($strGithubUser,$strGithubRepo)
		{
			$strLayoutDef = $this->GetLayoutHTML();
			
			$arrayItems = array();
			
			$strColorRibbon = $this->GetLayoutDef('LAYOUT_FORKMEONGITHUB_COLOR_RIBBON');
			if ($strColorRibbon == '')
			{
				$strColorRibbon = 'red';
			}
			
			$arrayItems['COLOR_RIBBON'] = $strColorRibbon;
			$arrayItems['GITHUB_USER'] = $strGithubUser;
			$arrayItems['GITHUB_REPO'] = $strGithubRepo;
			$arrayItems['TEXT'] = 'Fork me on GitHub';
			
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}
