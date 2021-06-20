<?php


	class CNavigationItem
	{
		public $m_nLevel;
		public $m_strText;
		public $m_strLink;
		public $m_bCurrentContext;
		public $m_bCurrentPath;
		public $m_bVisibleInPathMode;
		public $m_bConditionCompliance;
		public $m_bFirstInLevel;
		public $m_bLastInLevel;
		public $m_bURL;
		
		function IsExternalLink()
		{
			$strLink = $this->m_strLink;
			$bExternalLink = false;
			if (FindStringIgnoreCase($strLink,"http://") != -1)
			{
				$bExternalLink = true;
			}
			if (FindStringIgnoreCase($strLink,"https://") != -1)
			{
				$bExternalLink = true;
			}
			if (FindStringIgnoreCase($strLink,"mailto:") != -1)
			{
				$bExternalLink = true;
			}
			if (	(FindString($strLink,"{") == 0)
				&& (FindString($strLink,"}") == (StringLength($strLink)-1)))
			{
				$bExternalLink = true;
			}
			return $bExternalLink;
			
		}

		function GetURL(
			CWebsite $website,
			$strContextParameterName,
			$bSitemap = false,
			$strLanguage = '')
		{
			$strLink = $this->m_strLink;
			$bRealLink = $this->IsExternalLink();
			if ($bRealLink == false)
			{
				if (FindString($strLink,'#') == 0)
				{
					$bRealLink = true;
				}
			}

			if ($bRealLink == true)
			{
				$strURL = $strLink;
			}
			else
			{
				$arrayParams = array();
				$arrayParams[$strContextParameterName] = $strLink;
				if ($strLanguage != '')
				{
					$arrayParams['language'] = StringLowerCase($strLanguage);	
				}
				if ($bSitemap == true)
				{
					$strURL = $website->MakeSitemapURL($arrayParams);
				}
				else
				{
					$strURL = $website->MakeURL($arrayParams);
				}
			}
			return $strURL;
		}		
		
		
	}

	class CNavigationTree
	{
		private $m_arrayNavigationItems;
		private $m_strCurrentContextID;
		private $m_strContextParameterName;
		private $m_bCurrentPathInit;
		private $m_nIndexCurrentContext;
		private $m_nDeepestLevel;
		public  $m_nDeepestLevelVisibleInPathMode;
		public	$m_nCurrentContextLevel;
		
		function __construct()
		{
			$this->m_bCurrentPathInit = false;
			$this->m_arrayNavigationItems = array();
			$this->m_strContextParameterName = "content";
		}
		
		function IsInitCurrentPath()
		{
			return $this->m_bCurrentPathInit;
		}
				
		private
		function InitCurrentPath($strCurrentContextID)
		{
			$this->m_bCurrentPathInit = false;
			$this->m_nIndexCurrentContext = -1;
			

			for ($nIndex = 0; $nIndex < ArrayCount($this->m_arrayNavigationItems); $nIndex++)
			{
				if ($this->m_arrayNavigationItems[$nIndex]->m_strLink == $strCurrentContextID)
				{
					$this->m_arrayNavigationItems[$nIndex]->m_bCurrentContext = true;
					$this->m_arrayNavigationItems[$nIndex]->m_bCurrentPath = true;
					$this->m_nIndexCurrentContext = $nIndex;
					$this->m_nCurrentContextLevel = $this->m_arrayNavigationItems[$nIndex]->m_nLevel;
				}
				else
				{
					$this->m_arrayNavigationItems[$nIndex]->m_bCurrentContext = FALSE;
				}
			}

			// no current context was found in the Navigation -> only top level items are visible
			if ($this->m_nIndexCurrentContext == -1)
			{
				//printf("No current context<br>");
				for ($nIndex = 0; $nIndex < ArrayCount($this->m_arrayNavigationItems); $nIndex++)
				{
					if ($this->m_arrayNavigationItems[$nIndex]->m_nLevel == 0)
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bVisibleInPathMode = TRUE;
					}
					else
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bVisibleInPathMode = FALSE;
					}
				}

			}
			else // find the visible items
			{
				$this->m_arrayNavigationItems[$this->m_nIndexCurrentContext]->m_bVisibleInPathMode = TRUE;

				$entryPivot = $this->m_arrayNavigationItems[$this->m_nIndexCurrentContext];

				// Walk up the list
				$nCurrentLevel = $entryPivot->m_nLevel;
				for ($nIndex = $this->m_nIndexCurrentContext - 1; $nIndex >= 0; $nIndex--)
				{
					$entryCurrent = $this->m_arrayNavigationItems[$nIndex];
					if ($entryCurrent->m_nLevel <= $nCurrentLevel)
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bVisibleInPathMode = TRUE;
						$this->m_nDeepestLevelVisibleInPathMode = max($this->m_nDeepestLevelVisibleInPathMode,$entryCurrent->m_nLevel);
						if ($nCurrentLevel != $entryCurrent->m_nLevel)
						{
							$this->m_arrayNavigationItems[$nIndex]->m_bCurrentPath = TRUE;
						}
						$nCurrentLevel = $entryCurrent->m_nLevel;
					}

				}


				// Walk down the list
				$nCurrentLevel = $entryPivot->m_nLevel + 1;
				//$nFrendlyLevel = $nCurrentLevel + 1;
				for ($nIndex = $this->m_nIndexCurrentContext + 1; $nIndex < ArrayCount($this->m_arrayNavigationItems); $nIndex++)
				{
					$entryCurrent = $this->m_arrayNavigationItems[$nIndex];
					if ($entryCurrent->m_nLevel <= $nCurrentLevel)
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bVisibleInPathMode = TRUE;
						$this->m_nDeepestLevelVisibleInPathMode = max($this->m_nDeepestLevelVisibleInPathMode,$entryCurrent->m_nLevel);

						if ($this->m_arrayNavigationItems[$nIndex]->m_nLevel < $nCurrentLevel)
						{
							$nCurrentLevel = $entryCurrent->m_nLevel;
						}
					}
				}
			}
			
			$this->m_bCurrentPathInit = true;
			
			return $this->m_bCurrentPathInit;
		}


		function GetCurrentPath()
		{
			if (!$this->IsInitCurrentPath())
			{
				return false;	
			}

			$arrayCurrentPath = array();
			foreach ($this->m_arrayNavigationItems as $navItem)
			{
				if ($navItem->m_bCurrentPath)
				{
					$arrayCurrentPath[] = $navItem->m_strLink;
				}
			}
			return $arrayCurrentPath;
		}

	
		function GetNavigationFilePath(
			CWebsite &$website,
			$strNavigationID,
			$strLanguage = "")
		{
			if ($strLanguage == "")
			{
				$strLanguage = $website->GetLanguage();
			}
			$strNavigationFile = $website->GetAssemblyLine()->GetNavigationFilePath(
				$strNavigationID,
				$strLanguage);
			if ($strNavigationFile != false)
			{
				return $strNavigationFile;
			}
			//$website->Error("No Navigation file found for \"$strNavigationID\" in language \"$strLanguage\".");
			return "";
		}

		
		function LoadNavigationTree(
			CWebSite $website,
			$strNavigationID,
			$strLanguage = "")
		{
			$website->Trace("CNavigationTree::LoadNavigationTree('$strNavigationID')");	
			if ($strNavigationID == "")
			{
				$strNavigationID = "default";
			}
			
			$strFilePath = $this->GetNavigationFilePath(
				$website,
				$strNavigationID,
				$strLanguage);
			if ($strFilePath == "")
			{
				return false;	
			}
			
			$bResult = $this->LoadNavigationTreeFile(
				$website,
				$strFilePath);
			
			return $bResult;
		}
		
		
		
		function LoadNavigationTreeFile(
			CWebSite $website,
			$strFilePath)
		{
			$this->m_arrayNavigationItems = array();
			
			$arrayNavigationTreeRaw = FileReadCSV_WDK($strFilePath);
			
			if ($arrayNavigationTreeRaw == array())
			{
				return true;	
			}
			
			if ($arrayNavigationTreeRaw == false)
			{
				$website->Error("FileReadCSV_WDK($strFilePath) returned false");
				return false;
			}
		
			$nCountEntries = 0;
			

			foreach ($arrayNavigationTreeRaw as $arrayItemRaw)
			{
				//$website->ProfilingTrace();
				//$website->Trace("iterate items");
				
				$strLevel = ArrayGetValue($arrayItemRaw,"LEVEL");
				$nLevel = intval($strLevel);
				
				$strConditions = ArrayGetValue($arrayItemRaw,"CONDITIONS");
				$strConditions = StringTrim($strConditions);
				$arrayConditions = explode(",",$strConditions);
				
				$bConditionCompliance = true;
				foreach ($arrayConditions as $strCondition)
				{
					if ($strCondition != "")
					{
						$website->Trace("Condition: ".$strCondition);
						if ($website->CheckCondition($strCondition) == false)
						{
							$bConditionCompliance = false;
						}
					}
				}				
 
				$strContext = ArrayGetValue($arrayItemRaw,"CONTEXT");
				if (FindString($strContext,"include:") == 0)
				{
					$strIncludeNavID = StringSection($strContext,8);
					
					$navtree_include = $website->GetDynamicNavigationTree($strIncludeNavID);
					if (is_object($navtree_include) == false)
					{
						$navtree_include = new CNavigationTree();
						$navtree_include->LoadNavigationTree($website,$strIncludeNavID);
					}
					$this->AttachNavigationTree(
						$navtree_include,
						"", // attach to the (current) end
						$nLevel,
						$bConditionCompliance);
					continue;	
				}
				
				$strText = StringTrim(ArrayGetValue($arrayItemRaw,"LABEL"));



									
				$newEntry = new CNavigationItem(); 
				$newEntry->m_nLevel = $nLevel;
				$newEntry->m_strLink = $strContext;
				$newEntry->m_strText = $strText;

				$this->m_nDeepestLevel = max($this->m_nDeepestLevel,$nLevel);

				$newEntry->m_bConditionCompliance = $bConditionCompliance;

				$this->AttachItem($newEntry);
				$nCountEntries++;
			}

			// Subitems if items that do not match the conditions must also be marked conditioncompliance=false
			$nNonCompliantLevel = 0;
			$bMarkmode = false;
			for ($nIndex = 0; $nIndex < ArrayCount($this->m_arrayNavigationItems); $nIndex++)
			{
				//$this->GetWebsite()->Trace("BEFORE: $nIndex-".$this->m_arrayNavigationEntries[$nIndex]->m_nLevel."-$bMarkmode-$nNonCompliantLevel ".$this->m_arrayNavigationEntries[$nIndex]->m_strText." (".$this->m_arrayNavigationEntries[$nIndex]->m_bConditionCompliance.")");
				if ($bMarkmode == true)
				{
					if ($nNonCompliantLevel < $this->m_arrayNavigationItems[$nIndex]->m_nLevel)
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bConditionCompliance = false;
					}
					else
					{
						$bMarkmode = false;					
					}
				}
				if ($bMarkmode == false)
				{
					if ($this->m_arrayNavigationItems[$nIndex]->m_bConditionCompliance == false)
					{
						$bMarkmode = true;
						$nNonCompliantLevel = $this->m_arrayNavigationItems[$nIndex]->m_nLevel;
					}
				}
				//$this->GetWebsite()->Trace("AFTER: $nIndex-".$this->m_arrayNavigationEntries[$nIndex]->m_nLevel."-$bMarkmode-$nNonCompliantLevel ".$this->m_arrayNavigationEntries[$nIndex]->m_strText." (".$this->m_arrayNavigationEntries[$nIndex]->m_bConditionCompliance.")");
			}

			// Find the first and last item in any level
			$nPreviousLevel = 0;
			$nCount = ArrayCount($this->m_arrayNavigationItems);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$nCurrentLevel = $this->m_arrayNavigationItems[$nIndex]->m_nLevel;
				if ($nCurrentLevel > $nPreviousLevel)
				{
					$this->m_arrayNavigationItems[$nIndex]->m_bFirstInLevel = true;
				}
				if ($nCount-1 == $nIndex)
				{
					$this->m_arrayNavigationItems[$nIndex]->m_bLastInLevel = true;
				}
				else
				{
					$nNextLevel = $this->m_arrayNavigationItems[$nIndex+1]->m_nLevel;
					if ($nNextLevel < $nCurrentLevel)
					{
						$this->m_arrayNavigationItems[$nIndex]->m_bLastInLevel = true;
					}
				}
				$nPreviousLevel = $this->m_arrayNavigationItems[$nIndex]->m_nLevel;
			}
			return true;
		}		
		
		
		function SetCurrentContext(
			$strCurrentContextID)
		{
			$this->m_strCurrentContextID = $strCurrentContextID;
			$this->InitCurrentPath($strCurrentContextID);
		}
		
		function SetContextParameterName(
			$strContextParameterName)
		{
			$this->m_strContextParameterName = $strContextParameterName;
		}

		function GetContextParameterName()
		{
			return $this->m_strContextParameterName;
		}

		function AttachItem(CNavigationItem $newItem)
		{
			array_push($this->m_arrayNavigationItems,$newItem);
		}
		
		function AttachNavigationTree(
			CNavigationTree $navtree,
			$strContextID, // specify after which context id 
			$nLevel,
			$bConditionCompliance = true) 
		{
			// nLevel: 0 if the root level of the attached menu tree should be on the same level, 1 if the attached navigation tree should be on the level below the given context
			
			$this->m_bCurrentPathInit = false;
			
			if ($strContextID == "")
			{
				$arrayNavigationItemsNew = array();
				$arrayItemsToMerge = $navtree->GetNavigationItemArray();
				foreach ($arrayItemsToMerge as $newnavitem)
				{
					$newnavitem->m_nLevel += $nLevel;
					if ($bConditionCompliance == false)
					{
						$newnavitem->m_bConditionCompliance = false;
					}
					array_push($arrayNavigationItemsNew,$newnavitem);
				}

				$this->m_arrayNavigationItems = ArrayMerge(
					$this->m_arrayNavigationItems,
					$arrayItemsToMerge);
			}
			else
			{
				$arrayNavigationItemsNew = array();
				
				// Attach to the provided context node
				$nItems = ArrayCount($this->m_arrayNavigationItems);
				for ($nIndex = 0; $nIndex < $nItems; $nIndex++)
				{
					$navitem = $this->m_arrayNavigationItems[$nIndex];
					array_push($arrayNavigationItemsNew,$navitem);
					
					if ($strContextID == $navitem->m_strLink)
					{
						$nMergeLevel = $navitem->m_nLevel + max(1,$nLevel);
						$arrayItemsToMerge = $navtree->GetNavigationItemArray();
						foreach ($arrayItemsToMerge as $newnavitem)
						{
							$newnavitem->m_nLevel += $nMergeLevel;
							if ($bConditionCompliance == false)
							{
								$newnavitem->m_bConditionCompliance = false;
							}							
							array_push($arrayNavigationItemsNew,$newnavitem);
						}
					}
				}
				$this->m_arrayNavigationItems = $arrayNavigationItemsNew;
				
			}
			return true;		
		}
		
		function GetNavigationItemArray()
		{
			return $this->m_arrayNavigationItems;	
		}
		
		function SetNavigationItemArray($arrayNavigationItems)
		{
			$this->m_arrayNavigationItems = $arrayNavigationItems;	
		}		
		
		function GetPreviousItem()
		{
			$nCount = ArrayCount($this->m_arrayNavigationItems);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				if ($this->m_arrayNavigationItems[$nIndex]->m_bCurrentContext == true)
				{
					if ($nIndex == 0)	
					{
						return null;
					}
					else
					{
						return $this->m_arrayNavigationItems[$nIndex-1];
					}
				}
			}	
		}

		function GetNextItem()
		{
			$nCount = ArrayCount($this->m_arrayNavigationItems);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				if ($this->m_arrayNavigationItems[$nIndex]->m_bCurrentContext == true)
				{
					if ($nIndex == $nCount-1)	
					{
						return null;
					}
					else
					{
						return $this->m_arrayNavigationItems[$nIndex+1];
					}
				}
			}	
		}
		
		function FilterItemsForSitemap(CWebSite $website)
		{
			$arrayItems = array();
			foreach ($this->m_arrayNavigationItems as $navItem)
			{
				$strURL = $navItem->GetURL($website,"content",true);
				if (IsValidURL($strURL) && $navItem->m_bConditionCompliance)
				{
					$arrayItems[] = $navItem;
				}
			}
			$this->m_arrayNavigationItems = $arrayItems;
		}

	}



