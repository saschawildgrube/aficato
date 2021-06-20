<?php

	class CWikiElement extends CElement
	{
		protected $m_console;
		protected $m_formatbold;
		protected $m_formatitalic;
		protected $m_internallink;
		protected $m_externallink;
		protected $m_header;
		protected $m_list;
		protected $m_numberedlist;
		
		private $m_arrayTOC;
		private $m_arrayChapterIDs;
		
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$this->m_console = null;
			$this->m_formatbold = null;
			$this->m_formatitalic = null;
			$this->m_header = null;
			$this->m_internallink = null;
			$this->m_externallink = null;
			$this->m_list = null;
			$this->m_numberedlist = null;
			$this->m_definitionlist = null;
			$this->m_table = null;
			$this->m_arrayTOC = null;
			$this->m_arrayChapterIDs = array();

			parent::__construct(
				$website,
				"wiki",
				$strStyleID,
				true,
				false);
		}
		
		
		function RenderTable($arrayTableCells,$arrayTableHeaders)
		{
			if ($this->m_table == null)
			{
				$strLayoutStyle = $this->GetLayoutDef('LAYOUT_TABLE_STYLE');
				if ($strLayoutStyle == '')
				{
					$strLayoutStyle = 'default';
				}				
				$this->m_table = new CTableElement($this->GetWebsite(),$strLayoutStyle);
			}
			return $this->m_table->RenderTableEx($arrayTableCells,$arrayTableHeaders);
		}
		
		
		
		function RenderWiki($strSource,$strLinkTemplateInternal = "", $strChapterID = "", $strLinkTemplateChapter = "", CNavigationTree &$navigationtree = null/*, $bNumberedHeaders = false*/)
		{
			$this->m_arrayTOC = null;
			$strOutput = $strSource;
			$strOutput = trim($strOutput);
			
			if ($strLinkTemplateInternal == "")
			{ 
				$strLinkTemplateInternal = $this->GetWebsite()->MakeContentURL("{ARTICLEID}")."#{CHAPTERID}";
				$strLinkTemplateInternal = ReplaceString($strLinkTemplateInternal,"%7B",'{'); 
				$strLinkTemplateInternal = ReplaceString($strLinkTemplateInternal,"%7D",'}');
			}
			if (FindString($strLinkTemplateInternal,'{ARTICLEID}') == -1) 
			{
				$this->Trace("strLinkTemplateInternal = \"$strLinkTemplateInternal\"");
				$this->Error("CWikiElement::RenderWiki() called with invalid strLinkTemplate. strLinkTemplateInternal MUST contain \"{ARTICLEID}\".");
			}
		
			$bNumberedHeaders = GetBoolValue($this->GetLayoutDef("LAYOUT_HEADER_NUMBERED"));
			
			$bTOCSupport = false;
			
			if ($strChapterID == "TOC")
			{
				$bTOCSupport = true;
				if ($strLinkTemplateChapter == "")
				{
					$this->Trace("strLinkTemplateChapter = \"$strLinkTemplateChapter\"");
					$this->Error("CWikiElement::RenderWiki(): if strChapterID is set to \"TOC\" strLinkTemplateChapter must not be empty. How should the links to individual chapters be constructed without a link template?");
				}
			}
			
			$strLinkTemplateTOC = $strLinkTemplateChapter;
			
			if ($strLinkTemplateChapter == "")
			{
				$strLinkTemplateChapter = $strLinkTemplateInternal;
			}

			
			// pre tag support: on/off
			$arrayPreTagSections = explode("<pre>",$strOutput);
			
			$strOutput = "";
			$nTableLevel = 0;
			$arrayCurrentCell = array();
			$arrayCurrentCell[0] = "";
			$arrayTableCells = array();
			$arrayTableHeaders = array();
			$arrayTableRow = array();
			$arrayTableColumn = array();
			
			$nSection = 0;
			foreach ($arrayPreTagSections as $strPreTagSection)
			{
				$nPreTagClosePos = FindString($strPreTagSection,"</pre>");
				
				if ($nSection == 0)
				{
					$strPreContent = "";
					$strSection = trim($strPreTagSection);
				}
				else if ($nPreTagClosePos == -1)
				{
					$strPreContent = $strPreTagSection;
					$strSection = "";
				}
				else
				{
					$strPreContent = StringSection($strPreTagSection,0,$nPreTagClosePos);
					$strSection = trim(StringSection($strPreTagSection,$nPreTagClosePos+6));
				}

				if ($strPreContent != "")
				{
					if ($this->m_console == null)
					{
						$strLayoutStyle = $this->GetLayoutDef("LAYOUT_CONSOLE_STYLE");
						if ($strLayoutStyle == "")
						{
							$strLayoutStyle = "default";
						}
						$this->IncludeElement("console");
						$this->m_console= new CConsoleElement($this->GetWebsite(),$strLayoutStyle);
					}
					$arrayCurrentCell[$nTableLevel] .= $this->m_console->RenderConsole($strPreContent);
				}
				
				
				// Check for TOC tag
				$nOriginalSectionLength = StringLength($strSection);
				$strSection = ReplaceString($strSection,"__TOC__","{{__!_TOC_!__}}");
				
				$nNewSectionLength = StringLength($strSection); 
				if ($nNewSectionLength != $nOriginalSectionLength)
				{
					$bTOCSupport = true;
				}
				
				if ($bTOCSupport == true)
				{
					if ($navigationtree == null)
					{
						$navigationtree = new CNavigationTree();
					}
				}


				
				// Parse Links
				$strSection = $this->ParseInternalLinks($strSection,$strLinkTemplateInternal,$strLinkTemplateChapter);
				$strSection = $this->ParseExternalLinks($strSection);
				
				$arrayDefinitionList = array();
				$strLastTerm = "";
				
				$arrayListStack = array();
				$nStackLevel = 0;
				$strPrevMode = "";
	
				$arrayLines = explode("\n",$strSection);
				$arrayLines[] = ""; // add an empty line at the end to make sure the section after the last header section is rendered.
				$nLine = 0;
				$nLineCount = ArrayCount($arrayLines);
				foreach($arrayLines as $strLine)
				{
					$strUntrimmedLine = $strLine;
					$strLine = trim($strLine);
					$bEmptyLine = ($strLine=="")?(true):(false);
					$strMagicChar = StringSection($strLine,0,1);
					switch ($strMagicChar)
					{
						case "=":
							$strNewMode = "header";
							break;
						case "*":
						case "#":
							$strNewMode = "list";
							break;
						case ":":
						case ";":
							$strNewMode = "definitionlist";
							break;
						case "!":
							if ($nTableLevel > 0)
							{
								$strNewMode = "table_header";
								$strLine = StringSection($strLine,1);
							}
							else
							{
								$strNewMode = "text";		
								$strLine = $strUntrimmedLine;		
							}
							break;
						default:
							$strMagicCharPair = StringSection($strLine,0,2);
							switch ($strMagicCharPair)
							{
								case "{|":
									$strNewMode = "table_begin";
									$strLine = StringSection($strLine,2);
									break;
								case "|}":
									if ($nTableLevel > 0)
									{
										$strNewMode = "table_end";
										$strLine = StringSection($strLine,2);
									}
									else
									{
										$strNewMode = "text";		
										$strLine = $strUntrimmedLine;		
									}
									break;
								case "|-":
									if ($nTableLevel > 0)
									{
										$strNewMode = "table_row";
										$strLine = StringSection($strLine,2);
									}
									else
									{
										$strNewMode = "text";		
										$strLine = $strUntrimmedLine;		
									}
									break;
								default:
									if ($strMagicChar == "|")
									{
										if ($nTableLevel > 0)
										{
											$strNewMode = "table_cell";
											$strLine = StringSection($strLine,1);
										}
										else
										{
											$strNewMode = "text";		
											$strLine = $strUntrimmedLine;		
										}
									}
									else
									{
										$strNewMode = "text";		
										$strLine = $strUntrimmedLine;		
									}
									break;
							}
							break;
					}
					
					if ($strPrevMode != $strNewMode)
					{
						// Finish previous mode	
						if ($strPrevMode == "list")
						{
							$nLevel = 0;
							$arrayCurrentCell[$nTableLevel] .= $this->ParseList($arrayListStack,$nLevel,0) . "\n";
							$arrayListStack = array();
						}
						else if ($strPrevMode == "definitionlist")
						{
							$arrayCurrentCell[$nTableLevel] .= $this->ParseDefinitionList($arrayDefinitionList) . "\n";
							$arrayDefinitionList = array();
							$strLastTerm = "";
						}
					}
					
					if ($nLine < $nLineCount) // ignore the last empty line
					{
					
						switch ($strNewMode)
						{
							case "header":
								$arrayCurrentCell[$nTableLevel] .= $this->ParseHeaders($strLine,$strLinkTemplateTOC,$navigationtree,$strChapterID,$bNumberedHeaders) . "\n";
								break;
							case "list":
								$arrayListStack[] = $strLine;
								break;
							case "definitionlist":
								$strLine = trim(StringSection($strLine,1));
								if ($strMagicChar == ";")
								{
									$strLastTerm = $strLine;
								}
								else if ($strMagicChar == ":")
								{
									$arrayDefinitionList[$strLastTerm][] = $strLine;
								}
								break;
							default:
								if (StringSection($strLine,0,4) == "----")
								{
									$arrayCurrentCell[$nTableLevel] .= "<hr/>\n";
									$strLine = StringSection($strLine,5);
								}
								$strLine = $this->ParseFormats($strLine);
								
								// Table starts
								if ($strNewMode == "table_begin")
								{
									$nTableLevel++;
									$arrayTableCells[$nTableLevel] = array();
									$arrayTableHeaders[$nTableLevel] = array();
									$arrayTableRow[$nTableLevel] = 0;
									$arrayTableColumn[$nTableLevel] = -1;
									$arrayCurrentCell[$nTableLevel] = "";
								}
								// New Row
								else if ($strNewMode == "table_row")
								{
									if ($strPrevMode != "table_begin")
									{
										$arrayTableCells[$nTableLevel][ArrayGetValue($arrayTableRow,$nTableLevel)][ArrayGetValue($arrayTableColumn,$nTableLevel)] = ArrayGetValue($arrayCurrentCell,$nTableLevel);
										$arrayTableRow[$nTableLevel] = ArrayGetValue($arrayTableRow,$nTableLevel)+1;
										$arrayTableColumn[$nTableLevel] = -1;
										$arrayCurrentCell[$nTableLevel] = $strLine;
									}
								}
								// New Cell
								else if ($strNewMode == "table_header" || $strNewMode == "table_cell")
								{
//									if ($strPrevMode == "table_row" || $strPrevMode == "table_cell" || $strPrevMode == "table_header")
//									{
										$strRemainingLine = $strLine;
										do
										{
											if (ArrayGetValue($arrayTableColumn,$nTableLevel) > -1)
											{
												$arrayTableCells[$nTableLevel][ArrayGetValue($arrayTableRow,$nTableLevel)][ArrayGetValue($arrayTableColumn,$nTableLevel)] = ArrayGetValue($arrayCurrentCell,$nTableLevel);
											}
											$arrayTableColumn[$nTableLevel] = ArrayGetValue($arrayTableColumn,$nTableLevel)+1;
											$arrayTableHeaders[$nTableLevel][ArrayGetValue($arrayTableRow,$nTableLevel)][ArrayGetValue($arrayTableColumn,$nTableLevel)] = ($strNewMode == "table_header")?(true):(false);
											
											$strNextToken = "";
											$nNextCellPos = FindStringMultiple($strRemainingLine,array("||","!!"),$strNextToken);
											
											if ($nNextCellPos == -1)
											{
												$arrayCurrentCell[$nTableLevel] = $strRemainingLine;
												$strRemainingLine = "";
											}
											else
											{
												$arrayCurrentCell[$nTableLevel] = StringSection($strRemainingLine,0,$nNextCellPos);
												$strRemainingLine = StringSection($strRemainingLine,$nNextCellPos+2);
												if ($strNextToken == "!!")
												{
													$strNewMode == "table_header";
												}
												else
												{
													$strNewMode == "table_cell";
												}
											}
											
										} 
										while ($strRemainingLine != "");
	//								}
								}
								// Table ends (either explicitly or open end)
								else if ($strNewMode == "table_end")
								{
									if ($nTableLevel > 0)
									{
										if ($arrayTableColumn[$nTableLevel] > -1)
										{
											$arrayTableCells[$nTableLevel][$arrayTableRow[$nTableLevel]][$arrayTableColumn[$nTableLevel]] = $arrayCurrentCell[$nTableLevel];
										}
										$arrayCurrentCell[$nTableLevel-1] .= $this->RenderTable($arrayTableCells[$nTableLevel],$arrayTableHeaders[$nTableLevel]);
										$arrayTableCells[$nTableLevel] = array();
										$arrayTableHeaders[$nTableLevel] = array();
										$nTableLevel--;
									}
									$arrayCurrentCell[$nTableLevel] .= $strLine;
								}
								else
								{
									// A line break should only be added if
									// ...It's not the last line
									// ...The current section or cell is not empty
									if ($bEmptyLine && ($nLineCount != $nLine+1))
									{
										if ($arrayCurrentCell[$nTableLevel] != "")
										{
											$arrayCurrentCell[$nTableLevel] .= "<br/>\n";
										}
									}
									else
									{
										$arrayCurrentCell[$nTableLevel] .= $strLine . "\n";
									}	
								}
						}
					}
					$strPrevMode = $strNewMode;
					$nLine++;
				}
				$nSection++;
			}
			
			while ($nTableLevel > 0)
			{
				$arrayTableCells[$nTableLevel][$arrayTableRow[$nTableLevel]][$arrayTableColumn[$nTableLevel]] = $arrayCurrentCell[$nTableLevel];
				$arrayCurrentCell[$nTableLevel-1] .= $this->RenderTable($arrayTableCells[$nTableLevel],$arrayTableHeaders[$nTableLevel]);
				$nTableLevel--;
			}
			
			$strOutput .= $arrayCurrentCell[0];
			
			if ($bTOCSupport == true)
			{
				$strLayoutStyle = $this->GetLayoutDef("LAYOUT_NAVIGATION_STYLE");
				if ($strLayoutStyle == "")
				{
					$strLayoutStyle = "verticalpath";
				}
				$navigation = new CNavigationElement($this->GetWebsite(),$strLayoutStyle);
				$strTOC = $navigation->RenderNavigationVertical($navigationtree); 
				if ($strChapterID == "TOC")
				{
					$strOutput = $strTOC;
				}
				else
				{
					$strOutput = ReplaceString($strOutput,"{{__!_TOC_!__}}",$strTOC);  
				}
			}
			
			if ($strChapterID != "" && $strChapterID != "TOC")
			{
				$strChapterStartTag = "<!--".$strChapterID."-->";
				$nChapterStartTagLength = StringLength($strChapterStartTag);
				$nChapterStart = FindString($strOutput,$strChapterStartTag);
				if ($nChapterStart != -1)
				{
					$strOutput = StringSection($strOutput,$nChapterStart+$nChapterStartTagLength);
					$nChapterEnd = FindString($strOutput,"<!--CHAPTER-->");
					if ($nChapterEnd != -1)
					{
						$strOutput = StringSection($strOutput,0,$nChapterEnd);
					}
				}
			}
			else
			{
				$strOutput = ReplaceString($strOutput,"<!--CHAPTER-->","");	
			}
		
			return $strOutput;
		}

		private
		function ParseInternalLinks($strInput,$strLinkTemplate,$strLinkTemplateChapter)
		{
			$nFirstLinkPos = FindString($strInput,"[[");
			if ($nFirstLinkPos == -1)
			{
				return $strInput;	
			}
			
			$strOutput = StringSection($strInput,0,$nFirstLinkPos);
			$strRest = StringSection($strInput,$nFirstLinkPos+2);
			$arraySections = explode("[[",$strRest);	
			
			foreach ($arraySections as $strSection)
			{
				$nPosCloseTag = FindString($strSection,"]]");
				if ($nPosCloseTag == -1)
				{
					$strOutput .= "[[".$strSection;
					continue;
				}
				
				$strLinkRaw = StringSection($strSection,0,$nPosCloseTag);
				if ($strLinkRaw != "")
				{
					$nPosDelimiter = FindString($strLinkRaw,"|");
					if ($nPosDelimiter != -1)
					{
						$strArticleIDRaw = StringSection($strLinkRaw,0,$nPosDelimiter);
						$strText = StringSection($strLinkRaw,$nPosDelimiter+1);
					}
					else
					{
						$strArticleIDRaw = $strLinkRaw;
						$strText = $strLinkRaw;
					}
					$strText = trim($strText);
					
					if ($strArticleIDRaw != "")
					{
						if ($this->m_internallink == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_INTERNALLINK_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "default";
							}
							$this->m_internallink = new CLinkElement($this->GetWebsite(),$strLayoutStyle);
						}
						
						$strChapterIDRaw = "";
						$nPosHash = FindString($strArticleIDRaw,"#");
						if ($nPosHash != -1)
						{
							if ($nPosHash == 0)
							{
								$strText = 	StringSection($strArticleIDRaw,1);
							}
							$strChapterIDRaw = StringSection($strArticleIDRaw,$nPosHash+1); 
							$strArticleIDRaw = StringSection($strArticleIDRaw,0,$nPosHash);
						}
						
						$strArticleID = MakeID($strArticleIDRaw);
						if ($strArticleID == "")
						{
							$strArticleID = $this->GetWebsite()->GetContent();
						}
						
						if ($strArticleID == $this->GetWebsite()->GetContent())
						{
							//print("ABCD: ".$strLinkTemplateChapter);
							$strLinkTemplateInUse = $strLinkTemplateChapter;
						}
						else
						{
							$strLinkTemplateInUse = $strLinkTemplate;	
						}

						$strChapterID = MakeID($strChapterIDRaw);
						$strLink = ReplaceString($strLinkTemplateInUse,"{ARTICLEID}",$strArticleID);
						$strLink = ReplaceString($strLink,"{CHAPTERID}",$strChapterID);
						$strLinkHTML = $this->m_internallink->RenderLink($strLink,$strText);
						$strLinkHTML = trim($strLinkHTML);
						$strOutput .= $strLinkHTML . StringSection($strSection,$nPosCloseTag+2);
						continue;
					}
				}
				$strOutput .= "[[".$strSection;
			}
			return $strOutput;		
		}

		private
		function ParseExternalLinks($strInput)
		{
			$nFirstLinkPos = FindString($strInput,"[");
			if ($nFirstLinkPos == -1)
			{
				return $strInput;	
			}
			
			$strOutput = StringSection($strInput,0,$nFirstLinkPos);
			$arraySections = explode("[",StringSection($strInput,$nFirstLinkPos+1));	
			
			foreach ($arraySections as $strSection)
			{
				$nPosCloseTag = FindString($strSection,"]");
				if ($nPosCloseTag == -1)
				{
					$strOutput .= "[".$strSection;
					continue;
				}
				
				$strLinkRaw = StringSection($strSection,0,$nPosCloseTag);
				
				if ($strLinkRaw != "")
				{
					$nPosDelimiter = FindString($strLinkRaw,' ');
					if ($nPosDelimiter != -1)
					{
						$strURL = StringSection($strLinkRaw,0,$nPosDelimiter);
						$strText = StringSection($strLinkRaw,$nPosDelimiter+1);
					}
					else
					{
						$strURL = $strLinkRaw;
						$strText = $strLinkRaw;
					}
					$strText = trim($strText);
					
					$strURL = TweakURL($strURL);
					
					if (IsValidURL($strURL) || IsValidEmailURL($strURL))
					{
						$bExternal = GetBoolValue($this->GetLayoutDef("LAYOUT_EXTERNALLINK_EXTERNAL"));
						if ($this->m_externallink == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_EXTERNALLINK_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "default";
							}
							$this->m_externallink = new CLinkElement($this->GetWebsite(),$strLayoutStyle);
						}
						
						$strLinkHTML = $this->m_externallink->RenderLink($strURL,$strText,$bExternal);
						$strLinkHTML = trim($strLinkHTML);
						 
						$strOutput .= $strLinkHTML . StringSection($strSection,$nPosCloseTag+1);
						continue;
					}
					else
					{
						$this->Trace("strURL = $strURL");
						$this->Trace('IsValidURL() and IsValidEmailURL() returned false');
					}
				}
				$strOutput .= "[".$strSection;
			}
			return $strOutput;		
		}

		private
		function ParseDefinitionList($arrayList)
		{
			if ($this->m_definitionlist == null)
			{
				$strLayoutStyle = $this->GetLayoutDef("LAYOUT_DEFINITIONLIST_STYLE");
				if ($strLayoutStyle == "")
				{
					$strLayoutStyle = "default";
				}
				$this->IncludeElement("definitionlist");
				$this->m_definitionlist = new CDefinitionListElement($this->GetWebsite(),$strLayoutStyle);
			}
			return $this->m_definitionlist->RenderDefinitionList($arrayList);
		}

		
		private
		function ParseHeaders($strLine,$strLinkTemplate,CNavigationTree &$navigationtree = null, $strChapterID = "",$bNumberedHeaders = false)
		{
			$nTocLevel = intval($this->GetLayoutDef("LAYOUT_TOC_LEVEL"));
			if ($nTocLevel < 1)
			{
				$nTocLevel = 1;	
			}
			if ($nTocLevel > 6)
			{
				$nTocLevel = 6;	
			}
			
			if ($strLinkTemplate == "")
			{
				$strLinkTemplate ="#{CHAPTERID}";
			}
			$strLine = trim($strLine);
			for ($nLevel = 6; $nLevel >= 1; $nLevel--)
			{
				$strTag = StringRepeat("=",$nLevel);
				if (StringSection($strLine,0,$nLevel) == $strTag)
				{
					if (StringSection($strLine,-$nLevel) == $strTag)
					{
						if ($this->m_header == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_HEADER_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "default";
							}
							$this->IncludeElement("header");
							$this->m_header = new CHeaderElement($this->GetWebsite(),$strLayoutStyle);
						}
						
						$strNumber = "";
						if ($this->m_arrayTOC == null)
						{
							$this->m_arrayTOC = array();
						}
						if (ArrayKeyExists($this->m_arrayTOC,"".($nLevel)))
						{
							$arrayTOCNew = array();
							for ($nIndex = 1; $nIndex <= $nLevel; $nIndex++)
							{
								$arrayTOCNew["".$nIndex] = ArrayGetValue($this->m_arrayTOC,"".$nIndex);
							}
							$this->m_arrayTOC = $arrayTOCNew;
						}
						$this->m_arrayTOC["".$nLevel] = intval(ArrayGetValue($this->m_arrayTOC,"".$nLevel)) + 1;
						foreach ($this->m_arrayTOC as $nLevelNumber)
						{
							$strNumber .= "$nLevelNumber.";
						}

						
						
						$strHeaderText = StringSection($strLine,$nLevel,StringLength($strLine)-(2*$nLevel));
						$strHeaderTextNumbered = $strNumber . " " .$strHeaderText;
						// What to do in case of ambiguous chapter names?include the chapter number? Then links to chapters don't work
						$strCurrentChapterID = MakeID($strHeaderText);
						while (ArrayValueExists($this->m_arrayChapterIDs,$strCurrentChapterID) == true)
						{
							$strCurrentChapterID .= "-";
						}
						$this->m_arrayChapterIDs[] = $strCurrentChapterID;
						
						//$strCurrentChapterID = MakeID($strHeaderTextNumbered);
						
						
						$strHeader = "<!--CHAPTER-->";
						$strHeader .= "\n<div id=\"".$strCurrentChapterID."\"></div>\n";
						if ($strChapterID != "")
						{
							if ($strCurrentChapterID == $strChapterID)
							{
								$strHeader .= "<!--$strChapterID-->";
								$strHeaderText = $strHeaderTextNumbered;
							}
						}
						if ($bNumberedHeaders == true)
						{
							$strHeaderText = $strHeaderTextNumbered;
						}
						$strHeaderTextFormats = $this->ParseFormats($strHeaderText);
						$strHeader .= $this->m_header->RenderHeader($strHeaderTextFormats,$nLevel);
						 
						if ($navigationtree != null)
						{
							$newEntry = new CNavigationItem(); 
							$newEntry->m_nLevel = $nLevel - 1;
							$newEntry->m_strLink = ReplaceString($strLinkTemplate,"{CHAPTERID}",$strCurrentChapterID);
							if ($strChapterID != "")
							{
								if ($strCurrentChapterID == $strChapterID)
								{
									$newEntry->m_bCurrentContext = true;
								}
							}
							if ($nLevel <= $nTocLevel)
							{
								$newEntry->m_bConditionCompliance = true;
							}
							
							
							$newEntry->m_strText = $strHeaderTextNumbered;
							$navigationtree->AttachItem($newEntry);
						}
						
						return $strHeader;
					}	
				}
			}
			return $strLine;
		}
		
		private
		function ParseList($arrayListStack,&$nLine,$nLevel=0)
		{
			if (is_array($arrayListStack) == false)
			{
				return "";	
			}
			$strOutput = "";

			$nFirstLine = $nLine;
			$arrayListItems = array();
			$strPrevMode = "";			
			for (; $nLine < ArrayCount($arrayListStack); $nLine++)
			{
				$strLine = $arrayListStack[$nLine];
				$nLineLength = StringLength($strLine);
				
				$strModeString = "";
				$strText = "";
				for ($nPos = $nLevel; $nPos < $nLineLength; $nPos++)
				{
					if ($strLine[$nPos] == "*" || $strLine[$nPos] == "#")
					{
						$strModeString .= $strLine[$nPos];
					}
					else 
					{
						$strText = trim(StringSection($strLine,$nPos));
						break;
					}
				} 

				$strMode = StringSection($strModeString,0,1);
				$strSubMode = StringSection($strModeString,1,1);
			
				if ($strSubMode == "#" || $strSubMode == "*")
				{
					$strSubList = $this->ParseList($arrayListStack,$nLine,$nLevel+1);

					$strLastLine = array_pop($arrayListItems);
					$arrayListItems[] = $strLastLine . $strSubList;
					if ($nLine != $nFirstLine) 
					{
						$nLine--;
						$strText = "";
					}
				}
				else
				{
					if ($strPrevMode == "")
					{
						$strPrevMode = $strMode;	
					}				
					if ($strPrevMode == $strMode)
					{
						$arrayListItems[] = $this->ParseFormats($strText);
					}
				}

				if (	($strPrevMode != $strMode)
					|| ($nLine == ArrayCount($arrayListStack)-1)
					|| ($strPrevMode != "" && $strMode == ""))
				{
					if ($strPrevMode == "*")
					{
						if ($this->m_list == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_LIST_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "default";
							}
							$this->m_list = new CListElement($this->GetWebsite(),$strLayoutStyle);
						}
						$strOutput .= $this->m_list->RenderList($arrayListItems);
					}
					else
					{
						if ($this->m_numberedlist == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_NUMBEREDLIST_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "default";
							}
							$this->IncludeElement("numberedlist");
							$this->m_numberedlist = new CNumberedListElement($this->GetWebsite(),$strLayoutStyle);
						}
						$strOutput .= $this->m_numberedlist->RenderNumberedList($arrayListItems);				
					}
					$arrayListItems = array();
					if ($nLine != $nFirstLine && $strText != "")
					{  
						$arrayListItems[] = $this->ParseFormats($strText);
					}
				}
				
				if ($strMode == "")
				{
					return $strOutput;
				}
			
				// Consider the corner case: the last item is a different mode
				if ($nLine == ArrayCount($arrayListStack)-1)
				{
					if ($strMode != $strPrevMode)
					{
						$arrayListItems = array();
						$arrayListItems[] = $this->ParseFormats($strText);
						if ($strMode == "*")
						{
							if ($this->m_list == null)
							{
								$strLayoutStyle = $this->GetLayoutDef("LAYOUT_LIST_STYLE");
								if ($strLayoutStyle == "")
								{
									$strLayoutStyle = "default";
								}
								$this->m_list = new CListElement($this->GetWebsite(),$strLayoutStyle);
							}
							$strOutput .= $this->m_list->RenderList($arrayListItems);
						}
						else
						{
							if ($this->m_numberedlist == null)
							{
								$strLayoutStyle = $this->GetLayoutDef("LAYOUT_NUMBEREDLIST_STYLE");
								if ($strLayoutStyle == "")
								{
									$strLayoutStyle = "default";
								}
								$this->IncludeElement("numberedlist");
								$this->m_numberedlist = new CNumberedListElement($this->GetWebsite(),$strLayoutStyle);
							}
							$strOutput .= $this->m_numberedlist->RenderNumberedList($arrayListItems);				
						}

							
					}	
				}
				
				
				$strPrevMode = $strMode;
			}
			return $strOutput;
		}
		
		private
		function ParseFormats($strInput)
		{
			if (FindString($strInput,"'") == -1)
			{
				return $strInput;	
			}
			
			$this->IncludeElement("format");
			
			$arraySections = preg_split( "/(''+)/", $strInput, -1, PREG_SPLIT_DELIM_CAPTURE );
			if (ArrayCount($arraySections) == 1)
			{
				return $strInput;	
			}
			
			$strOutput = "";
			$strMode = "";
			$strPrevMode = "";
			$nIndex = 0;
			$nSectionCount = ArrayCount($arraySections);
			$strPrevSection = "";
			foreach ($arraySections as $strSection)
			{
				$bTagSection = (StringSection($strSection,0,2) == "''")?(true):(false);
				$strPrevMode = $strMode;
				if ($bTagSection)
				{
					// this is a format tag: let's change the mode and add the last section
					$nLength = StringLength($strSection);
					if ($nLength == 2) // italic
					{
						if ($strMode == "")
						{
							$strMode = "i";
						}
						else if ($strMode == "b")
						{
							$strMode = "ib";	
						}
						else if ($strMode == "ib")
						{
							$strMode = "b";	
						}
						else // e.g. strMode == "i"
						{
							$strMode = "";	
						}
					}
					else if ($nLength == 3) // bold
					{
						if ($strMode == "")
						{
							$strMode = "b";
						}
						else if ($strMode == "i")
						{
							$strMode = "ib";	
						}
						else if ($strMode == "ib")
						{
							$strMode = "i";	
						}
						else // e.g. strMode == "b"
						{
							$strMode = "";	
						}
					}
					else if ($nLength == 5) // italic and bold
					{
						if ($strMode == "")
						{
							$strMode = "ib";
						}
						else if ($strMode == "i")
						{
							$strMode = "b";	
						}
						else if ($strMode == "b")
						{
							$strMode = "i";	
						}
						else // e.g. strMode == "ib"
						{
							$strMode = "";	
						}							
					}
					else 
					{
						// any higher amount of quotes is ignored
					}
				}	
				else
				{
					// this is a text section
					$strPrevSection = $strSection;
				}
				
				if ($bTagSection || $nIndex == $nSectionCount-1 )
				{
					$strSectionOutput = $strPrevSection;
					if (FindString($strPrevMode,"i") != -1)
					{
						if ($this->m_formatitalic == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_FORMATITALIC_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "italic";
							}
							$this->m_formatitalic = new CFormatElement($this->GetWebsite(),$strLayoutStyle);
						} 
						$strSectionOutput = trim($this->m_formatitalic->RenderFormat($strSectionOutput));
					}
					if (FindString($strPrevMode,"b") != -1)
					{
						if ($this->m_formatbold == null)
						{
							$strLayoutStyle = $this->GetLayoutDef("LAYOUT_FORMATBOLD_STYLE");
							if ($strLayoutStyle == "")
							{
								$strLayoutStyle = "bold";
							}
							$this->m_formatbold = new CFormatElement($this->GetWebsite(),$strLayoutStyle);
						}
						$strSectionOutput = trim($this->m_formatbold->RenderFormat($strSectionOutput));
					}
					$strOutput .= $strSectionOutput;
				}
				$nIndex++;
			}
			return $strOutput;	
		}
	}

