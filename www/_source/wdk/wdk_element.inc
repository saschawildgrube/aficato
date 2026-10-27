<?php

	require_once('wdk_cache.inc');

	abstract class CElement extends CWebsiteSatellite
	{
		private $m_strElementID;
		private $m_strStyleID;
		
		private $m_bLayoutDef;
		private $m_arrayLayoutDef;
		
		private $m_bLayoutCFG;
		private $m_bLayoutHTML;
		private $m_strLayoutHTML;

		private static $m_cacheLayout = null;
		
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			parent::__construct($website,$strElementID);

			if (self::$m_cacheLayout == null)
			{
				self::$m_cacheLayout = new CMemoryCache();
			}
			
			// Init values
			$this->m_bLayoutCFG = $bLayoutCFG;
			$this->m_bLayoutHTML = $bLayoutHTML;
			$this->m_arrayLayoutDef = array();
			$this->m_strElementID = $strElementID;
			$this->SetStyle($strStyleID);
		}
		
		function GetSatelliteType()
		{
			return 'Element';	
		}

		function SetStyle($strNewStyleID)
		{
			if ($strNewStyleID == '')
			{
				$this->Error('CElement: Style is missing!');	
			}
			
			if ($strNewStyleID != $this->m_strStyleID)	
			{
				$this->m_strStyleID = $strNewStyleID;
				$this->LoadLayoutFile();
			}
		}
		
		function GetStyle()
		{
			return $this->m_strStyleID;	
		}
		
		function GetElementID()
		{
			return $this->m_strElementID;
		}
		
		function LoadLayoutFile()
		{
			if ($this->m_strStyleID != '')
			{
				$strLayoutCacheID = $this->GetWebsite()->GetLayout()."\t".$this->m_strElementID."\t".$this->m_strStyleID;
				$nLayoutCacheTTL = 3600;
				$bLayoutCFG = false;
				$bLayoutHTML = false;
				if ($this->m_bLayoutCFG == true)
				{
					$arrayLayoutDef = self::$m_cacheLayout->GetCacheObject("CFG\t".$strLayoutCacheID);
					if ($arrayLayoutDef !== false)
					{
						$this->m_arrayLayoutDef = $arrayLayoutDef;
						$bLayoutCFG = true;
					}
					else
					{
						$strFileCFG = $this->GetAssemblyLine()->GetElementLayoutCfgFilePath(
							$this->GetWebsite()->GetLayout(),
							$this->m_strElementID,
							$this->m_strStyleID);
						$this->Trace("CElement::LoadLayoutFile: CFG: \"$strFileCFG\"");		
						if ($strFileCFG != false)
						{
							$this->m_arrayLayoutDef = LoadNamedArrayFromTextFileMultiLine($strFileCFG,'LAYOUT_');
							self::$m_cacheLayout->SetCacheObject(
								"CFG\t".$strLayoutCacheID,
								$this->m_arrayLayoutDef,
								$nLayoutCacheTTL);
							$bLayoutCFG = true;
						}
						else
						{
							$this->Error('No layout CFG file found for style "'.$this->m_strStyleID.'"');	
						}
					}					
				}
				if ($this->m_bLayoutHTML == true) 
				{
					$strLayoutHTML = self::$m_cacheLayout->GetCacheObject("HTML\t".$strLayoutCacheID);
					if ($strLayoutHTML !== false)
					{
						$this->m_strLayoutHTML = $strLayoutHTML;
						$bLayoutHTML = true;
					}
					else
					{
						$strFileHTML = $this->GetAssemblyLine()->GetElementLayoutHtmlFilePath(
							$this->GetWebsite()->GetLayout(),
							$this->m_strElementID,
							$this->m_strStyleID);
						$this->Trace("CElement::LoadLayoutFile: HTML: \"$strFileHTML\"");			
						if ($strFileHTML != false)
						{
							$this->m_strLayoutHTML = FileRead($strFileHTML);
							self::$m_cacheLayout->SetCacheObject(
								"HTML\t".$strLayoutCacheID,
								$this->m_strLayoutHTML,
								$nLayoutCacheTTL);
							$bLayoutHTML = true;
						}
						else
						{
							$this->Error('No layout HTML file found for style "'.$this->m_strStyleID.'"');	
						}
					}
				} 
			}
		}
		
		function GetLayoutDef($strLayoutDef)
		{
			return ArrayGetValue($this->m_arrayLayoutDef,$strLayoutDef);
		}
		
		function GetLayoutHTML()
		{
			return $this->m_strLayoutHTML;
		}

	}

