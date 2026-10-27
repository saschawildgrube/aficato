<?php

	require_once(GetWDKDir()."wdk_module.inc");

	class CStaticFileViewerModule extends CModule
	{
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct($website,"cms/staticfileviewer",$strInstanceID,$arrayConfig);
		}

		static function IsTextFile($strPath)
		{
			$strExtension = strtolower(pathinfo($strPath,PATHINFO_EXTENSION));
			return in_array($strExtension,array(
				'txt','text','log','csv','tsv','md','markdown','rst','adoc','tex',
				'bat','cmd','ps1','psm1','psd1','sh','bash','zsh','fish',
				'php','php3','php4','php5','php7','php8','phtml','inc','phps',
				'html','htm','xhtml','css','scss','sass','less','js','mjs','cjs','jsx','ts','tsx',
				'json','jsonc','jsonl','xml','xsl','xslt','xsd','svg','yaml','yml','toml',
				'ini','conf','config','cfg','properties','env','htaccess','htpasswd',
				'py','pyw','rb','pl','pm','lua','r','sql','graphql','gql',
				'c','h','cc','cpp','cxx','hpp','hxx','cs','java','kt','kts','scala',
				'go','rs','swift','m','mm','vb','vbs','asp','aspx','jsp','vue','svelte',
				'dart','ex','exs','erl','hrl','hs','clj','cljs','lisp','cl','el',
				'asm','s','pas','pp','f','f90','cmake','make','mk','gradle','groovy',
				'tpl','twig','mustache','hbs','diff','patch','gitignore','gitattributes',
				'editorconfig','dockerignore'),true);
		}

		static function ResolveFile($strRootDir,$strFile)
		{
			if ($strRootDir == "" || strpos($strFile,"\0") !== false)
			{
				return false;
			}
			$strRoot = realpath($strRootDir);
			if ($strRoot === false || !is_dir($strRoot))
			{
				return false;
			}
			$strRoot = rtrim($strRoot,"/\\").DIRECTORY_SEPARATOR;
			$strPath = realpath($strRoot.$strFile);
			if ($strPath === false || !is_file($strPath) || !is_readable($strPath))
			{
				return false;
			}
			$bInsideRoot = (DIRECTORY_SEPARATOR == "\\")
				? strncasecmp($strPath,$strRoot,strlen($strRoot)) == 0
				: strncmp($strPath,$strRoot,strlen($strRoot)) == 0;
			return $bInsideRoot ? $strPath : false;
		}

		static function EncodeText($strText)
		{
			// Keep source tokens literal during subsequent WDK template/resource passes.
			return str_replace(array('{','}','?'),array('&#123;','&#125;','&#63;'),HtmlEncode($strText));
		}

		function OnRenderModule($strState)
		{
			$this->GetWebsite()->AddStyleSheet("wdk-module-staticfileviewer");
			$this->SetOutputItem("STATICFILEVIEWER_FILENAME","");
			$this->SetOutputItem("STATICFILEVIEWER_TEXT","");
			$strFile = $this->GetModuleConfig("file");
			if ($strFile == "")
			{
				$strFile = $this->GetModuleVar("file");
			}
			$strPath = self::ResolveFile($this->GetModuleConfig("rootdir"),$strFile);
			if ($strPath === false || !self::IsTextFile($strFile) || !self::IsTextFile($strPath))
			{
				$this->SetErrorMessageCode("FILE_INVALID");
				return "view";
			}
			$strText = @file_get_contents($strPath);
			if ($strText === false)
			{
				$this->SetErrorMessageCode("FILE_INVALID");
				return "view";
			}
			$this->SetOutputItem("STATICFILEVIEWER_FILENAME",self::EncodeText(basename($strFile)));
			$this->SetOutputItem("STATICFILEVIEWER_TEXT",self::EncodeText($strText));
			return "view";
		}
	}
