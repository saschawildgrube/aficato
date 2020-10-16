<?php

	class CMailIconElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'mailicon',
				$strStyleID,
				false,
				true);
		}
		
		function RenderMailIcon($strMail, $strToolTip, $strSubject = '', $strBody = '')
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			$strMailURL = 'mailto:'.$strMail.'?';
			$strOptions = '';
			if ($strSubject != '')
			{
				$strOptions .= 'subject='.StringURLEncode($strSubject);
			}
			if ($strBody != '')
			{
				if ($strOptions != '')                
				{  
					$strOptions .= '&amp;';
				}
				$strOptions .= 'body='.StringURLEncode($strBody);
			}
			$strMailURL .= $strOptions;
			$arrayItems['MAILURL'] = $strMailURL;
			$arrayItems['TOOLTIP'] = $strToolTip;
			
			$strOutput = ReplaceTagsWDK($strOutput, $arrayItems);
			
			return $strOutput;
		}
	}

