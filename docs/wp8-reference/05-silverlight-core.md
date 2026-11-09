# System.Windows (core types: FrameworkElement, UIElement, Visual states, input, media…)

From the WP8 SDK IntelliSense XML.

## Contents

- [`Microsoft.Internal.IManagedFrameworkInternalHelper`](#microsoftinternalimanagedframeworkinternalhelper)
- [`Microsoft.Xna.Framework.Silverlight.GraphicsDeviceManager`](#microsoftxnaframeworksilverlightgraphicsdevicemanager)
- [`Microsoft.Xna.Framework.Silverlight.RenderMode`](#microsoftxnaframeworksilverlightrendermode)
- [`Microsoft.Xna.Framework.Silverlight.RenderModeChangedEventArgs`](#microsoftxnaframeworksilverlightrendermodechangedeventargs)
- [`Microsoft.Xna.Framework.Silverlight.RenderModeReason`](#microsoftxnaframeworksilverlightrendermodereason)
- [`System.Collections.ObjectModel.ObservableCollection`1`](#systemcollectionsobjectmodelobservablecollection1)
- [`System.Collections.ObjectModel.ReadOnlyObservableCollection`1`](#systemcollectionsobjectmodelreadonlyobservablecollection1)
- [`System.Collections.Specialized.INotifyCollectionChanged`](#systemcollectionsspecializedinotifycollectionchanged)
- [`System.Collections.Specialized.NotifyCollectionChangedAction`](#systemcollectionsspecializednotifycollectionchangedaction)
- [`System.Collections.Specialized.NotifyCollectionChangedEventArgs`](#systemcollectionsspecializednotifycollectionchangedeventargs)
- [`System.Collections.Specialized.NotifyCollectionChangedEventHandler`](#systemcollectionsspecializednotifycollectionchangedeventhandler)
- [`System.ComponentModel.AlternateContentPropertyAttribute`](#systemcomponentmodelalternatecontentpropertyattribute)
- [`System.ComponentModel.ClosingEventArgs`](#systemcomponentmodelclosingeventargs)
- [`System.ComponentModel.CurrentChangingEventArgs`](#systemcomponentmodelcurrentchangingeventargs)
- [`System.ComponentModel.CurrentChangingEventHandler`](#systemcomponentmodelcurrentchangingeventhandler)
- [`System.ComponentModel.DataErrorsChangedEventArgs`](#systemcomponentmodeldataerrorschangedeventargs)
- [`System.ComponentModel.DesignerProperties`](#systemcomponentmodeldesignerproperties)
- [`System.ComponentModel.GroupDescription`](#systemcomponentmodelgroupdescription)
- [`System.ComponentModel.ICollectionView`](#systemcomponentmodelicollectionview)
- [`System.ComponentModel.ICollectionViewFactory`](#systemcomponentmodelicollectionviewfactory)
- [`System.ComponentModel.IEditableCollectionView`](#systemcomponentmodelieditablecollectionview)
- [`System.ComponentModel.INotifyDataErrorInfo`](#systemcomponentmodelinotifydataerrorinfo)
- [`System.ComponentModel.NewItemPlaceholderPosition`](#systemcomponentmodelnewitemplaceholderposition)
- [`System.ComponentModel.SortDescription`](#systemcomponentmodelsortdescription)
- [`System.ComponentModel.SortDescriptionCollection`](#systemcomponentmodelsortdescriptioncollection)
- [`System.IO.IsolatedStorage.IsolatedStorageSettings`](#systemioisolatedstorageisolatedstoragesettings)
- [`System.Net.Browser.WebRequestCreator`](#systemnetbrowserwebrequestcreator)
- [`System.Net.HttpUtility`](#systemnethttputility)
- [`System.Reflection.ICustomTypeProvider`](#systemreflectionicustomtypeprovider)
- [`System.Runtime.InteropServices.Automation.AutomationEvent`](#systemruntimeinteropservicesautomationautomationevent)
- [`System.Runtime.InteropServices.Automation.AutomationEventArgs`](#systemruntimeinteropservicesautomationautomationeventargs)
- [`System.Runtime.InteropServices.Automation.AutomationFactory`](#systemruntimeinteropservicesautomationautomationfactory)
- [`System.Windows.Analytics`](#systemwindowsanalytics)
- [`System.Windows.Application`](#systemwindowsapplication)
- [`System.Windows.ApplicationServiceContext`](#systemwindowsapplicationservicecontext)
- [`System.Windows.ApplicationUnhandledExceptionEventArgs`](#systemwindowsapplicationunhandledexceptioneventargs)
- [`System.Windows.AssemblyPart`](#systemwindowsassemblypart)
- [`System.Windows.AssemblyPartCollection`](#systemwindowsassemblypartcollection)
- [`System.Windows.Automation.AutomationElementIdentifiers`](#systemwindowsautomationautomationelementidentifiers)
- [`System.Windows.Automation.AutomationProperties`](#systemwindowsautomationautomationproperties)
- [`System.Windows.Automation.AutomationProperty`](#systemwindowsautomationautomationproperty)
- [`System.Windows.Automation.AutomationTextAttribute`](#systemwindowsautomationautomationtextattribute)
- [`System.Windows.Automation.DockPatternIdentifiers`](#systemwindowsautomationdockpatternidentifiers)
- [`System.Windows.Automation.DockPosition`](#systemwindowsautomationdockposition)
- [`System.Windows.Automation.ElementNotAvailableException`](#systemwindowsautomationelementnotavailableexception)
- [`System.Windows.Automation.ElementNotEnabledException`](#systemwindowsautomationelementnotenabledexception)
- [`System.Windows.Automation.ExpandCollapsePatternIdentifiers`](#systemwindowsautomationexpandcollapsepatternidentifiers)
- [`System.Windows.Automation.ExpandCollapseState`](#systemwindowsautomationexpandcollapsestate)
- [`System.Windows.Automation.GridItemPatternIdentifiers`](#systemwindowsautomationgriditempatternidentifiers)
- [`System.Windows.Automation.GridPatternIdentifiers`](#systemwindowsautomationgridpatternidentifiers)
- [`System.Windows.Automation.MultipleViewPatternIdentifiers`](#systemwindowsautomationmultipleviewpatternidentifiers)
- [`System.Windows.Automation.Peers.AutomationControlType`](#systemwindowsautomationpeersautomationcontroltype)
- [`System.Windows.Automation.Peers.AutomationEvents`](#systemwindowsautomationpeersautomationevents)
- [`System.Windows.Automation.Peers.AutomationOrientation`](#systemwindowsautomationpeersautomationorientation)
- [`System.Windows.Automation.Peers.AutomationPeer`](#systemwindowsautomationpeersautomationpeer)
- [`System.Windows.Automation.Peers.ButtonAutomationPeer`](#systemwindowsautomationpeersbuttonautomationpeer)
- [`System.Windows.Automation.Peers.ButtonBaseAutomationPeer`](#systemwindowsautomationpeersbuttonbaseautomationpeer)
- [`System.Windows.Automation.Peers.CheckBoxAutomationPeer`](#systemwindowsautomationpeerscheckboxautomationpeer)
- [`System.Windows.Automation.Peers.ComboBoxAutomationPeer`](#systemwindowsautomationpeerscomboboxautomationpeer)
- [`System.Windows.Automation.Peers.FrameworkElementAutomationPeer`](#systemwindowsautomationpeersframeworkelementautomationpeer)
- [`System.Windows.Automation.Peers.HyperlinkButtonAutomationPeer`](#systemwindowsautomationpeershyperlinkbuttonautomationpeer)
- [`System.Windows.Automation.Peers.ImageAutomationPeer`](#systemwindowsautomationpeersimageautomationpeer)
- [`System.Windows.Automation.Peers.ItemAutomationPeer`](#systemwindowsautomationpeersitemautomationpeer)
- [`System.Windows.Automation.Peers.ItemsControlAutomationPeer`](#systemwindowsautomationpeersitemscontrolautomationpeer)
- [`System.Windows.Automation.Peers.ListBoxAutomationPeer`](#systemwindowsautomationpeerslistboxautomationpeer)
- [`System.Windows.Automation.Peers.ListBoxItemAutomationPeer`](#systemwindowsautomationpeerslistboxitemautomationpeer)
- [`System.Windows.Automation.Peers.MediaElementAutomationPeer`](#systemwindowsautomationpeersmediaelementautomationpeer)
- [`System.Windows.Automation.Peers.MultiScaleImageAutomationPeer`](#systemwindowsautomationpeersmultiscaleimageautomationpeer)
- [`System.Windows.Automation.Peers.PasswordBoxAutomationPeer`](#systemwindowsautomationpeerspasswordboxautomationpeer)
- [`System.Windows.Automation.Peers.PatternInterface`](#systemwindowsautomationpeerspatterninterface)
- [`System.Windows.Automation.Peers.ProgressBarAutomationPeer`](#systemwindowsautomationpeersprogressbarautomationpeer)
- [`System.Windows.Automation.Peers.RadioButtonAutomationPeer`](#systemwindowsautomationpeersradiobuttonautomationpeer)
- [`System.Windows.Automation.Peers.RangeBaseAutomationPeer`](#systemwindowsautomationpeersrangebaseautomationpeer)
- [`System.Windows.Automation.Peers.RepeatButtonAutomationPeer`](#systemwindowsautomationpeersrepeatbuttonautomationpeer)
- [`System.Windows.Automation.Peers.RichTextBoxAutomationPeer`](#systemwindowsautomationpeersrichtextboxautomationpeer)
- [`System.Windows.Automation.Peers.ScrollBarAutomationPeer`](#systemwindowsautomationpeersscrollbarautomationpeer)
- [`System.Windows.Automation.Peers.ScrollViewerAutomationPeer`](#systemwindowsautomationpeersscrollviewerautomationpeer)
- [`System.Windows.Automation.Peers.SelectorAutomationPeer`](#systemwindowsautomationpeersselectorautomationpeer)
- [`System.Windows.Automation.Peers.SelectorItemAutomationPeer`](#systemwindowsautomationpeersselectoritemautomationpeer)
- [`System.Windows.Automation.Peers.SliderAutomationPeer`](#systemwindowsautomationpeerssliderautomationpeer)
- [`System.Windows.Automation.Peers.TextBlockAutomationPeer`](#systemwindowsautomationpeerstextblockautomationpeer)
- [`System.Windows.Automation.Peers.TextBoxAutomationPeer`](#systemwindowsautomationpeerstextboxautomationpeer)
- [`System.Windows.Automation.Peers.ThumbAutomationPeer`](#systemwindowsautomationpeersthumbautomationpeer)
- [`System.Windows.Automation.Peers.ToggleButtonAutomationPeer`](#systemwindowsautomationpeerstogglebuttonautomationpeer)
- [`System.Windows.Automation.Provider.IDockProvider`](#systemwindowsautomationprovideridockprovider)
- [`System.Windows.Automation.Provider.IExpandCollapseProvider`](#systemwindowsautomationprovideriexpandcollapseprovider)
- [`System.Windows.Automation.Provider.IGridItemProvider`](#systemwindowsautomationproviderigriditemprovider)
- [`System.Windows.Automation.Provider.IGridProvider`](#systemwindowsautomationproviderigridprovider)
- [`System.Windows.Automation.Provider.IInvokeProvider`](#systemwindowsautomationprovideriinvokeprovider)
- [`System.Windows.Automation.Provider.IMultipleViewProvider`](#systemwindowsautomationproviderimultipleviewprovider)
- [`System.Windows.Automation.Provider.IRangeValueProvider`](#systemwindowsautomationproviderirangevalueprovider)
- [`System.Windows.Automation.Provider.IRawElementProviderSimple`](#systemwindowsautomationproviderirawelementprovidersimple)
- [`System.Windows.Automation.Provider.IScrollItemProvider`](#systemwindowsautomationprovideriscrollitemprovider)
- [`System.Windows.Automation.Provider.IScrollProvider`](#systemwindowsautomationprovideriscrollprovider)
- [`System.Windows.Automation.Provider.ISelectionItemProvider`](#systemwindowsautomationprovideriselectionitemprovider)
- [`System.Windows.Automation.Provider.ISelectionProvider`](#systemwindowsautomationprovideriselectionprovider)
- [`System.Windows.Automation.Provider.ITableItemProvider`](#systemwindowsautomationprovideritableitemprovider)
- [`System.Windows.Automation.Provider.ITableProvider`](#systemwindowsautomationprovideritableprovider)
- [`System.Windows.Automation.Provider.ITextProvider`](#systemwindowsautomationprovideritextprovider)
- [`System.Windows.Automation.Provider.ITextRangeProvider`](#systemwindowsautomationprovideritextrangeprovider)
- [`System.Windows.Automation.Provider.IToggleProvider`](#systemwindowsautomationprovideritoggleprovider)
- [`System.Windows.Automation.Provider.ITransformProvider`](#systemwindowsautomationprovideritransformprovider)
- [`System.Windows.Automation.Provider.IValueProvider`](#systemwindowsautomationproviderivalueprovider)
- [`System.Windows.Automation.Provider.IWindowProvider`](#systemwindowsautomationprovideriwindowprovider)
- [`System.Windows.Automation.RangeValuePatternIdentifiers`](#systemwindowsautomationrangevaluepatternidentifiers)
- [`System.Windows.Automation.RowOrColumnMajor`](#systemwindowsautomationroworcolumnmajor)
- [`System.Windows.Automation.ScrollAmount`](#systemwindowsautomationscrollamount)
- [`System.Windows.Automation.ScrollPatternIdentifiers`](#systemwindowsautomationscrollpatternidentifiers)
- [`System.Windows.Automation.SelectionItemPatternIdentifiers`](#systemwindowsautomationselectionitempatternidentifiers)
- [`System.Windows.Automation.SelectionPatternIdentifiers`](#systemwindowsautomationselectionpatternidentifiers)
- [`System.Windows.Automation.SupportedTextSelection`](#systemwindowsautomationsupportedtextselection)
- [`System.Windows.Automation.TableItemPatternIdentifiers`](#systemwindowsautomationtableitempatternidentifiers)
- [`System.Windows.Automation.TablePatternIdentifiers`](#systemwindowsautomationtablepatternidentifiers)
- [`System.Windows.Automation.Text.TextPatternRangeEndpoint`](#systemwindowsautomationtexttextpatternrangeendpoint)
- [`System.Windows.Automation.Text.TextUnit`](#systemwindowsautomationtexttextunit)
- [`System.Windows.Automation.TextPatternIdentifiers`](#systemwindowsautomationtextpatternidentifiers)
- [`System.Windows.Automation.TogglePatternIdentifiers`](#systemwindowsautomationtogglepatternidentifiers)
- [`System.Windows.Automation.ToggleState`](#systemwindowsautomationtogglestate)
- [`System.Windows.Automation.TransformPatternIdentifiers`](#systemwindowsautomationtransformpatternidentifiers)
- [`System.Windows.Automation.ValuePatternIdentifiers`](#systemwindowsautomationvaluepatternidentifiers)
- [`System.Windows.Automation.WindowInteractionState`](#systemwindowsautomationwindowinteractionstate)
- [`System.Windows.Automation.WindowPatternIdentifiers`](#systemwindowsautomationwindowpatternidentifiers)
- [`System.Windows.Automation.WindowVisualState`](#systemwindowsautomationwindowvisualstate)
- [`System.Windows.CheckAndDownloadUpdateCompletedEventArgs`](#systemwindowscheckanddownloadupdatecompletedeventargs)
- [`System.Windows.CheckAndDownloadUpdateCompletedEventHandler`](#systemwindowscheckanddownloadupdatecompletedeventhandler)
- [`System.Windows.Clipboard`](#systemwindowsclipboard)
- [`System.Windows.CornerRadius`](#systemwindowscornerradius)
- [`System.Windows.CrossDomainAccess`](#systemwindowscrossdomainaccess)
- [`System.Windows.Data.Binding`](#systemwindowsdatabinding)
- [`System.Windows.Data.BindingBase`](#systemwindowsdatabindingbase)
- [`System.Windows.Data.BindingExpression`](#systemwindowsdatabindingexpression)
- [`System.Windows.Data.BindingExpressionBase`](#systemwindowsdatabindingexpressionbase)
- [`System.Windows.Data.BindingMode`](#systemwindowsdatabindingmode)
- [`System.Windows.Data.BindingOperations`](#systemwindowsdatabindingoperations)
- [`System.Windows.Data.CollectionViewGroup`](#systemwindowsdatacollectionviewgroup)
- [`System.Windows.Data.CollectionViewSource`](#systemwindowsdatacollectionviewsource)
- [`System.Windows.Data.Debugging.BindingAction`](#systemwindowsdatadebuggingbindingaction)
- [`System.Windows.Data.Debugging.BindingDebugState`](#systemwindowsdatadebuggingbindingdebugstate)
- [`System.Windows.Data.Debugging.BindingStage`](#systemwindowsdatadebuggingbindingstage)
- [`System.Windows.Data.Debugging.BindingValuePipeline`](#systemwindowsdatadebuggingbindingvaluepipeline)
- [`System.Windows.Data.FilterEventArgs`](#systemwindowsdatafiltereventargs)
- [`System.Windows.Data.FilterEventHandler`](#systemwindowsdatafiltereventhandler)
- [`System.Windows.Data.IValueConverter`](#systemwindowsdataivalueconverter)
- [`System.Windows.Data.PropertyGroupDescription`](#systemwindowsdatapropertygroupdescription)
- [`System.Windows.Data.RelativeSource`](#systemwindowsdatarelativesource)
- [`System.Windows.Data.RelativeSourceMode`](#systemwindowsdatarelativesourcemode)
- [`System.Windows.Data.UpdateSourceTrigger`](#systemwindowsdataupdatesourcetrigger)
- [`System.Windows.DataFormats`](#systemwindowsdataformats)
- [`System.Windows.DataObject`](#systemwindowsdataobject)
- [`System.Windows.DataTemplate`](#systemwindowsdatatemplate)
- [`System.Windows.DataTemplateKey`](#systemwindowsdatatemplatekey)
- [`System.Windows.DependencyObject`](#systemwindowsdependencyobject)
- [`System.Windows.DependencyObjectCollection`1`](#systemwindowsdependencyobjectcollection1)
- [`System.Windows.DependencyProperty`](#systemwindowsdependencyproperty)
- [`System.Windows.DependencyPropertyChangedEventArgs`](#systemwindowsdependencypropertychangedeventargs)
- [`System.Windows.DependencyPropertyChangedEventHandler`](#systemwindowsdependencypropertychangedeventhandler)
- [`System.Windows.Deployment`](#systemwindowsdeployment)
- [`System.Windows.Documents.Block`](#systemwindowsdocumentsblock)
- [`System.Windows.Documents.BlockCollection`](#systemwindowsdocumentsblockcollection)
- [`System.Windows.Documents.Bold`](#systemwindowsdocumentsbold)
- [`System.Windows.Documents.FontSource`](#systemwindowsdocumentsfontsource)
- [`System.Windows.Documents.Glyphs`](#systemwindowsdocumentsglyphs)
- [`System.Windows.Documents.Hyperlink`](#systemwindowsdocumentshyperlink)
- [`System.Windows.Documents.Inline`](#systemwindowsdocumentsinline)
- [`System.Windows.Documents.InlineCollection`](#systemwindowsdocumentsinlinecollection)
- [`System.Windows.Documents.InlineUIContainer`](#systemwindowsdocumentsinlineuicontainer)
- [`System.Windows.Documents.Italic`](#systemwindowsdocumentsitalic)
- [`System.Windows.Documents.LineBreak`](#systemwindowsdocumentslinebreak)
- [`System.Windows.Documents.LogicalDirection`](#systemwindowsdocumentslogicaldirection)
- [`System.Windows.Documents.Paragraph`](#systemwindowsdocumentsparagraph)
- [`System.Windows.Documents.Run`](#systemwindowsdocumentsrun)
- [`System.Windows.Documents.Section`](#systemwindowsdocumentssection)
- [`System.Windows.Documents.Span`](#systemwindowsdocumentsspan)
- [`System.Windows.Documents.TextElement`](#systemwindowsdocumentstextelement)
- [`System.Windows.Documents.TextElementCollection`1`](#systemwindowsdocumentstextelementcollection1)
- [`System.Windows.Documents.TextPointer`](#systemwindowsdocumentstextpointer)
- [`System.Windows.Documents.TextSelection`](#systemwindowsdocumentstextselection)
- [`System.Windows.Documents.Underline`](#systemwindowsdocumentsunderline)
- [`System.Windows.DragEventArgs`](#systemwindowsdrageventargs)
- [`System.Windows.DragEventHandler`](#systemwindowsdrageventhandler)
- [`System.Windows.Duration`](#systemwindowsduration)
- [`System.Windows.DurationConverter`](#systemwindowsdurationconverter)
- [`System.Windows.ElevatedPermissions`](#systemwindowselevatedpermissions)
- [`System.Windows.EventTrigger`](#systemwindowseventtrigger)
- [`System.Windows.ExceptionRoutedEventArgs`](#systemwindowsexceptionroutedeventargs)
- [`System.Windows.ExceptionRoutedEventHandler`](#systemwindowsexceptionroutedeventhandler)
- [`System.Windows.Expression`](#systemwindowsexpression)
- [`System.Windows.ExtensionPart`](#systemwindowsextensionpart)
- [`System.Windows.ExternalPart`](#systemwindowsexternalpart)
- [`System.Windows.ExternalPartCollection`](#systemwindowsexternalpartcollection)
- [`System.Windows.FlowDirection`](#systemwindowsflowdirection)
- [`System.Windows.FontStretch`](#systemwindowsfontstretch)
- [`System.Windows.FontStretches`](#systemwindowsfontstretches)
- [`System.Windows.FontStyle`](#systemwindowsfontstyle)
- [`System.Windows.FontStyles`](#systemwindowsfontstyles)
- [`System.Windows.FontWeight`](#systemwindowsfontweight)
- [`System.Windows.FontWeights`](#systemwindowsfontweights)
- [`System.Windows.FrameworkElement`](#systemwindowsframeworkelement)
- [`System.Windows.FrameworkTemplate`](#systemwindowsframeworktemplate)
- [`System.Windows.GpuInformation`](#systemwindowsgpuinformation)
- [`System.Windows.GridLength`](#systemwindowsgridlength)
- [`System.Windows.GridUnitType`](#systemwindowsgridunittype)
- [`System.Windows.HorizontalAlignment`](#systemwindowshorizontalalignment)
- [`System.Windows.IApplicationLifetimeAware`](#systemwindowsiapplicationlifetimeaware)
- [`System.Windows.IApplicationService`](#systemwindowsiapplicationservice)
- [`System.Windows.IDataObject`](#systemwindowsidataobject)
- [`System.Windows.Icon`](#systemwindowsicon)
- [`System.Windows.IconCollection`](#systemwindowsiconcollection)
- [`System.Windows.Ink.DrawingAttributes`](#systemwindowsinkdrawingattributes)
- [`System.Windows.Ink.Stroke`](#systemwindowsinzrnroke)
- [`System.Windows.Ink.StrokeCollection`](#systemwindowsinzrnrokecollection)
- [`System.Windows.Input.Cursor`](#systemwindowsinputcursor)
- [`System.Windows.Input.Cursors`](#systemwindowsinputcursors)
- [`System.Windows.Input.FocusManager`](#systemwindowsinputfocusmanager)
- [`System.Windows.Input.GestureEventArgs`](#systemwindowsinputgestureeventargs)
- [`System.Windows.Input.ICommand`](#systemwindowsinputicommand)
- [`System.Windows.Input.ImeConversionModeValues`](#systemwindowsinputimeconversionmodevalues)
- [`System.Windows.Input.InputMethod`](#systemwindowsinputinputmethod)
- [`System.Windows.Input.InputMethodState`](#systemwindowsinputinputmethodstate)
- [`System.Windows.Input.InputScope`](#systemwindowsinputinputscope)
- [`System.Windows.Input.InputScopeConverter`](#systemwindowsinputinputscopeconverter)
- [`System.Windows.Input.InputScopeName`](#systemwindowsinputinputscopename)
- [`System.Windows.Input.InputScopeNameConverter`](#systemwindowsinputinputscopenameconverter)
- [`System.Windows.Input.InputScopeNameValue`](#systemwindowsinputinputscopenamevalue)
- [`System.Windows.Input.Zey`](#systemwindowsinputkey)
- [`System.Windows.Input.ZeyEventArgs`](#systemwindowsinputkeyeventargs)
- [`System.Windows.Input.ZeyEventHandler`](#systemwindowsinputkeyeventhandler)
- [`System.Windows.Input.Zeyboard`](#systemwindowsinputkeyboard)
- [`System.Windows.Input.ZeyboardNavigationMode`](#systemwindowsinputkeyboardnavigationmode)
- [`System.Windows.Input.ManipulationCompletedEventArgs`](#systemwindowsinputmanipulationcompletedeventargs)
- [`System.Windows.Input.ManipulationDelta`](#systemwindowsinputmanipulationdelta)
- [`System.Windows.Input.ManipulationDeltaEventArgs`](#systemwindowsinputmanipulationdeltaeventargs)
- [`System.Windows.Input.ManipulationStartedEventArgs`](#systemwindowsinputmanipulationstartedeventargs)
- [`System.Windows.Input.ManipulationVelocities`](#systemwindowsinputmanipulationvelocities)
- [`System.Windows.Input.ModifierKeys`](#systemwindowsinputmodifierkeys)
- [`System.Windows.Input.MouseButtonEventArgs`](#systemwindowsinputmousebuttoneventargs)
- [`System.Windows.Input.MouseButtonEventHandler`](#systemwindowsinputmousebuttoneventhandler)
- [`System.Windows.Input.MouseEventArgs`](#systemwindowsinputmouseeventargs)
- [`System.Windows.Input.MouseEventHandler`](#systemwindowsinputmouseeventhandler)
- [`System.Windows.Input.MouseWheelEventArgs`](#systemwindowsinputmousewheeleventargs)
- [`System.Windows.Input.MouseWheelEventHandler`](#systemwindowsinputmousewheeleventhandler)
- [`System.Windows.Input.StylusDevice`](#systemwindowsinputstylusdevice)
- [`System.Windows.Input.StylusPoint`](#systemwindowsinputstyluspoint)
- [`System.Windows.Input.StylusPointCollection`](#systemwindowsinputstyluspointcollection)
- [`System.Windows.Input.TabletDeviceType`](#systemwindowsinputtabletdevicetype)
- [`System.Windows.Input.TextComposition`](#systemwindowsinputtextcomposition)
- [`System.Windows.Input.TextCompositionEventArgs`](#systemwindowsinputtextcompositioneventargs)
- [`System.Windows.Input.TextCompositionEventHandler`](#systemwindowsinputtextcompositioneventhandler)
- [`System.Windows.Input.Touch`](#systemwindowsinputtouch)
- [`System.Windows.Input.TouchAction`](#systemwindowsinputtouchaction)
- [`System.Windows.Input.TouchDevice`](#systemwindowsinputtouchdevice)
- [`System.Windows.Input.TouchFrameEventArgs`](#systemwindowsinputtouchframeeventargs)
- [`System.Windows.Input.TouchFrameEventHandler`](#systemwindowsinputtouchframeeventhandler)
- [`System.Windows.Input.TouchPoint`](#systemwindowsinputtouchpoint)
- [`System.Windows.Input.TouchPointCollection`](#systemwindowsinputtouchpointcollection)
- [`System.Windows.InstallState`](#systemwindowsinstallstate)
- [`System.Windows.Interop.Content`](#systemwindowsinteropcontent)
- [`System.Windows.Interop.FullScreenOptions`](#systemwindowsinteropfullscreenoptions)
- [`System.Windows.Interop.HostingRenderTargetBitmap`](#systemwindowsinterophostingrendertargetbitmap)
- [`System.Windows.Interop.NavigationStateChangedEventArgs`](#systemwindowsinteropnavigationstatechangedeventargs)
- [`System.Windows.Interop.Settings`](#systemwindowsinteropsettings)
- [`System.Windows.Interop.SilverlightHost`](#systemwindowsinteropsilverlighthost)
- [`System.Windows.LayoutCycleException`](#systemwindowslayoutcycleexception)
- [`System.Windows.LineStackingStrategy`](#systemwindowslinestackingstrategy)
- [`System.Windows.Markup.ContentPropertyAttribute`](#systemwindowsmarkupcontentpropertyattribute)
- [`System.Windows.Markup.IProvideValueTarget`](#systemwindowsmarkupiprovidevaluetarget)
- [`System.Windows.Markup.IXamlTypeResolver`](#systemwindowsmarkupixamltyperesolver)
- [`System.Windows.Markup.MarkupExtension`](#systemwindowsmarkupmarkupextension)
- [`System.Windows.Markup.XamlParseException`](#systemwindowsmarkupxamlparseexception)
- [`System.Windows.Markup.XamlReader`](#systemwindowsmarkupxamlreader)
- [`System.Windows.Markup.XmlLanguage`](#systemwindowsmarkupxmllanguage)
- [`System.Windows.Markup.XmlnsDefinitionAttribute`](#systemwindowsmarkupxmlnsdefinitionattribute)
- [`System.Windows.Markup.XmlnsPrefixAttribute`](#systemwindowsmarkupxmlnsprefixattribute)
- [`System.Windows.Media.AcquireLicenseCompletedEventArgs`](#systemwindowsmediaacquirelicensecompletedeventargs)
- [`System.Windows.Media.AlignmentX`](#systemwindowsmediaalignmentx)
- [`System.Windows.Media.AlignmentY`](#systemwindowsmediaalignmenty)
- [`System.Windows.Media.Animation.BackEase`](#systemwindowsmediaanimationbackease)
- [`System.Windows.Media.Animation.BeginStoryboard`](#systemwindowsmediaanimationbeginstoryboard)
- [`System.Windows.Media.Animation.BounceEase`](#systemwindowsmediaanimationbounceease)
- [`System.Windows.Media.Animation.CircleEase`](#systemwindowsmediaanimationcircleease)
- [`System.Windows.Media.Animation.ClockState`](#systemwindowsmediaanimationcloczrnate)
- [`System.Windows.Media.Animation.ColorAnimation`](#systemwindowsmediaanimationcoloranimation)
- [`System.Windows.Media.Animation.ColorAnimationUsingKeyFrames`](#systemwindowsmediaanimationcoloranimationusingkeyframes)
- [`System.Windows.Media.Animation.ColorKeyFrame`](#systemwindowsmediaanimationcolorkeyframe)
- [`System.Windows.Media.Animation.ColorKeyFrameCollection`](#systemwindowsmediaanimationcolorkeyframecollection)
- [`System.Windows.Media.Animation.CubicEase`](#systemwindowsmediaanimationcubicease)
- [`System.Windows.Media.Animation.DiscreteColorKeyFrame`](#systemwindowsmediaanimationdiscretecolorkeyframe)
- [`System.Windows.Media.Animation.DiscreteDoubleKeyFrame`](#systemwindowsmediaanimationdiscretedoublekeyframe)
- [`System.Windows.Media.Animation.DiscreteObjectKeyFrame`](#systemwindowsmediaanimationdiscreteobjectkeyframe)
- [`System.Windows.Media.Animation.DiscretePointKeyFrame`](#systemwindowsmediaanimationdiscretepointkeyframe)
- [`System.Windows.Media.Animation.DoubleAnimation`](#systemwindowsmediaanimationdoubleanimation)
- [`System.Windows.Media.Animation.DoubleAnimationUsingKeyFrames`](#systemwindowsmediaanimationdoubleanimationusingkeyframes)
- [`System.Windows.Media.Animation.DoubleKeyFrame`](#systemwindowsmediaanimationdoublekeyframe)
- [`System.Windows.Media.Animation.DoubleKeyFrameCollection`](#systemwindowsmediaanimationdoublekeyframecollection)
- [`System.Windows.Media.Animation.EasingColorKeyFrame`](#systemwindowsmediaanimationeasingcolorkeyframe)
- [`System.Windows.Media.Animation.EasingDoubleKeyFrame`](#systemwindowsmediaanimationeasingdoublekeyframe)
- [`System.Windows.Media.Animation.EasingFunctionBase`](#systemwindowsmediaanimationeasingfunctionbase)
- [`System.Windows.Media.Animation.EasingMode`](#systemwindowsmediaanimationeasingmode)
- [`System.Windows.Media.Animation.EasingPointKeyFrame`](#systemwindowsmediaanimationeasingpointkeyframe)
- [`System.Windows.Media.Animation.ElasticEase`](#systemwindowsmediaanimationelasticease)
- [`System.Windows.Media.Animation.ExponentialEase`](#systemwindowsmediaanimationexponentialease)
- [`System.Windows.Media.Animation.FillBehavior`](#systemwindowsmediaanimationfillbehavior)
- [`System.Windows.Media.Animation.IEasingFunction`](#systemwindowsmediaanimationieasingfunction)
- [`System.Windows.Media.Animation.ZeySpline`](#systemwindowsmediaanimationkeyspline)
- [`System.Windows.Media.Animation.ZeyTime`](#systemwindowsmediaanimationkeytime)
- [`System.Windows.Media.Animation.ZeyTimeType`](#systemwindowsmediaanimationkeytimetype)
- [`System.Windows.Media.Animation.LinearColorKeyFrame`](#systemwindowsmediaanimationlinearcolorkeyframe)
- [`System.Windows.Media.Animation.LinearDoubleKeyFrame`](#systemwindowsmediaanimationlineardoublekeyframe)
- [`System.Windows.Media.Animation.LinearPointKeyFrame`](#systemwindowsmediaanimationlinearpointkeyframe)
- [`System.Windows.Media.Animation.ObjectAnimationUsingKeyFrames`](#systemwindowsmediaanimationobjectanimationusingkeyframes)
- [`System.Windows.Media.Animation.ObjectKeyFrame`](#systemwindowsmediaanimationobjectkeyframe)
- [`System.Windows.Media.Animation.ObjectKeyFrameCollection`](#systemwindowsmediaanimationobjectkeyframecollection)
- [`System.Windows.Media.Animation.PointAnimation`](#systemwindowsmediaanimationpointanimation)
- [`System.Windows.Media.Animation.PointAnimationUsingKeyFrames`](#systemwindowsmediaanimationpointanimationusingkeyframes)
- [`System.Windows.Media.Animation.PointKeyFrame`](#systemwindowsmediaanimationpointkeyframe)
- [`System.Windows.Media.Animation.PointKeyFrameCollection`](#systemwindowsmediaanimationpointkeyframecollection)
- [`System.Windows.Media.Animation.PowerEase`](#systemwindowsmediaanimationpowerease)
- [`System.Windows.Media.Animation.QuadraticEase`](#systemwindowsmediaanimationquadraticease)
- [`System.Windows.Media.Animation.QuarticEase`](#systemwindowsmediaanimationquarticease)
- [`System.Windows.Media.Animation.QuinticEase`](#systemwindowsmediaanimationquinticease)
- [`System.Windows.Media.Animation.RepeatBehavior`](#systemwindowsmediaanimationrepeatbehavior)
- [`System.Windows.Media.Animation.SineEase`](#systemwindowsmediaanimationsineease)
- [`System.Windows.Media.Animation.SplineColorKeyFrame`](#systemwindowsmediaanimationsplinecolorkeyframe)
- [`System.Windows.Media.Animation.SplineDoubleKeyFrame`](#systemwindowsmediaanimationsplinedoublekeyframe)
- [`System.Windows.Media.Animation.SplinePointKeyFrame`](#systemwindowsmediaanimationsplinepointkeyframe)
- [`System.Windows.Media.Animation.Storyboard`](#systemwindowsmediaanimationstoryboard)
- [`System.Windows.Media.Animation.Timeline`](#systemwindowsmediaanimationtimeline)
- [`System.Windows.Media.Animation.TimelineCollection`](#systemwindowsmediaanimationtimelinecollection)
- [`System.Windows.Media.ArcSegment`](#systemwindowsmediaarcsegment)
- [`System.Windows.Media.AudioCaptureDevice`](#systemwindowsmediaaudiocapturedevice)
- [`System.Windows.Media.AudioCaptureDeviceCollection`](#systemwindowsmediaaudiocapturedevicecollection)
- [`System.Windows.Media.AudioFormat`](#systemwindowsmediaaudioformat)
- [`System.Windows.Media.AudioSink`](#systemwindowsmediaaudiosink)
- [`System.Windows.Media.BezierSegment`](#systemwindowsmediabeziersegment)
- [`System.Windows.Media.BitmapCache`](#systemwindowsmediabitmapcache)
- [`System.Windows.Media.Brush`](#systemwindowsmediabrush)
- [`System.Windows.Media.BrushMappingMode`](#systemwindowsmediabrushmappingmode)
- [`System.Windows.Media.CacheMode`](#systemwindowsmediacachemode)
- [`System.Windows.Media.CaptureDevice`](#systemwindowsmediacapturedevice)
- [`System.Windows.Media.CaptureDeviceConfiguration`](#systemwindowsmediacapturedeviceconfiguration)
- [`System.Windows.Media.CaptureImageCompletedEventArgs`](#systemwindowsmediacaptureimagecompletedeventargs)
- [`System.Windows.Media.CaptureSource`](#systemwindowsmediacapturesource)
- [`System.Windows.Media.CaptureState`](#systemwindowsmediacapturestate)
- [`System.Windows.Media.Color`](#systemwindowsmediacolor)
- [`System.Windows.Media.ColorInterpolationMode`](#systemwindowsmediacolorinterpolationmode)
- [`System.Windows.Media.Colors`](#systemwindowsmediacolors)
- [`System.Windows.Media.CompositeTransform`](#systemwindowsmediacompositetransform)
- [`System.Windows.Media.CompositionTarget`](#systemwindowsmediacompositiontarget)
- [`System.Windows.Media.ContentKeyType`](#systemwindowsmediacontentkeytype)
- [`System.Windows.Media.DeepZoomImageTileSource`](#systemwindowsmediadeepzoomimagetilesource)
- [`System.Windows.Media.DomainAcquirer`](#systemwindowsmediadomainacquirer)
- [`System.Windows.Media.DomainOperationCompletedEventArgs`](#systemwindowsmediadomainoperationcompletedeventargs)
- [`System.Windows.Media.DoubleCollection`](#systemwindowsmediadoublecollection)
- [`System.Windows.Media.Effects.BlurEffect`](#systemwindowsmediaeffectsblureffect)
- [`System.Windows.Media.Effects.DropShadowEffect`](#systemwindowsmediaeffectsdropshadoweffect)
- [`System.Windows.Media.Effects.Effect`](#systemwindowsmediaeffectseffect)
- [`System.Windows.Media.Effects.PixelShader`](#systemwindowsmediaeffectspixelshader)
- [`System.Windows.Media.Effects.SamplingMode`](#systemwindowsmediaeffectssamplingmode)
- [`System.Windows.Media.Effects.ShaderEffect`](#systemwindowsmediaeffectsshadereffect)
- [`System.Windows.Media.EllipseGeometry`](#systemwindowsmediaellipsegeometry)
- [`System.Windows.Media.FileSink`](#systemwindowsmediafilesink)
- [`System.Windows.Media.FillRule`](#systemwindowsmediafillrule)
- [`System.Windows.Media.FontFamily`](#systemwindowsmediafontfamily)
- [`System.Windows.Media.Fonts`](#systemwindowsmediafonts)
- [`System.Windows.Media.GeneralTransform`](#systemwindowsmediageneraltransform)
- [`System.Windows.Media.Geometry`](#systemwindowsmediageometry)
- [`System.Windows.Media.GeometryCollection`](#systemwindowsmediageometrycollection)
- [`System.Windows.Media.GeometryGroup`](#systemwindowsmediageometrygroup)
- [`System.Windows.Media.GlyphTypeface`](#systemwindowsmediaglyphtypeface)
- [`System.Windows.Media.GradientBrush`](#systemwindowsmediagradientbrush)
- [`System.Windows.Media.GradientSpreadMethod`](#systemwindowsmediagradientspreadmethod)
- [`System.Windows.Media.GradientStop`](#systemwindowsmediagradientstop)
- [`System.Windows.Media.GradientStopCollection`](#systemwindowsmediagradientstopcollection)
- [`System.Windows.Media.ImageBrush`](#systemwindowsmediaimagebrush)
- [`System.Windows.Media.ImageSource`](#systemwindowsmediaimagesource)
- [`System.Windows.Media.ImageSourceConverter`](#systemwindowsmediaimagesourceconverter)
- [`System.Windows.Media.Imaging.BitmapCreateOptions`](#systemwindowsmediaimagingbitmapcreateoptions)
- [`System.Windows.Media.Imaging.BitmapImage`](#systemwindowsmediaimagingbitmapimage)
- [`System.Windows.Media.Imaging.BitmapSource`](#systemwindowsmediaimagingbitmapsource)
- [`System.Windows.Media.Imaging.BitmapSourceExtensions`](#systemwindowsmediaimagingbitmapsourceextensions)
- [`System.Windows.Media.Imaging.DownloadProgressEventArgs`](#systemwindowsmediaimagingdownloadprogresseventargs)
- [`System.Windows.Media.Imaging.WriteableBitmap`](#systemwindowsmediaimagingwriteablebitmap)
- [`System.Windows.Media.ImplicitInputBrush`](#systemwindowsmediaimplicitinputbrush)
- [`System.Windows.Media.JoinDomainCompletedEventArgs`](#systemwindowsmediajoindomaincompletedeventargs)
- [`System.Windows.Media.LeaveDomainCompletedEventArgs`](#systemwindowsmedialeavedomaincompletedeventargs)
- [`System.Windows.Media.LicenseAcquirer`](#systemwindowsmedialicenseacquirer)
- [`System.Windows.Media.LicenseManagement`](#systemwindowsmedialicensemanagement)
- [`System.Windows.Media.LineGeometry`](#systemwindowsmedialinegeometry)
- [`System.Windows.Media.LineSegment`](#systemwindowsmedialinesegment)
- [`System.Windows.Media.LinearGradientBrush`](#systemwindowsmedialineargradientbrush)
- [`System.Windows.Media.LogReadyRoutedEventArgs`](#systemwindowsmedialogreadyroutedeventargs)
- [`System.Windows.Media.LogReadyRoutedEventHandler`](#systemwindowsmedialogreadyroutedeventhandler)
- [`System.Windows.Media.LogSource`](#systemwindowsmedialogsource)
- [`System.Windows.Media.Matrix`](#systemwindowsmediamatrix)
- [`System.Windows.Media.Matrix3DProjection`](#systemwindowsmediamatrix3dprojection)
- [`System.Windows.Media.MatrixTransform`](#systemwindowsmediamatrixtransform)
- [`System.Windows.Media.Media3D.Matrix3D`](#systemwindowsmediamedia3dmatrix3d)
- [`System.Windows.Media.MediaElementState`](#systemwindowsmediamediaelementstate)
- [`System.Windows.Media.MediaLicense`](#systemwindowsmediamedialicense)
- [`System.Windows.Media.MediaSampleAttributeKeys`](#systemwindowsmediamediasampleattributekeys)
- [`System.Windows.Media.MediaSourceAttributesKeys`](#systemwindowsmediamediasourceattributeskeys)
- [`System.Windows.Media.MediaStreamAttributeKeys`](#systemwindowsmediamediastreamattributekeys)
- [`System.Windows.Media.MediaStreamDescription`](#systemwindowsmediamediastreamdescription)
- [`System.Windows.Media.MediaStreamSample`](#systemwindowsmediamediastreamsample)
- [`System.Windows.Media.MediaStreamSource`](#systemwindowsmediamediastreamsource)
- [`System.Windows.Media.MediaStreamSourceDiagnosticKind`](#systemwindowsmediamediastreamsourcediagnostickind)
- [`System.Windows.Media.MediaStreamType`](#systemwindowsmediamediastreamtype)
- [`System.Windows.Media.MultiScaleTileSource`](#systemwindowsmediamultiscaletilesource)
- [`System.Windows.Media.PathFigure`](#systemwindowsmediapathfigure)
- [`System.Windows.Media.PathFigureCollection`](#systemwindowsmediapathfigurecollection)
- [`System.Windows.Media.PathGeometry`](#systemwindowsmediapathgeometry)
- [`System.Windows.Media.PathSegment`](#systemwindowsmediapathsegment)
- [`System.Windows.Media.PathSegmentCollection`](#systemwindowsmediapathsegmentcollection)
- [`System.Windows.Media.PenLineCap`](#systemwindowsmediapenlinecap)
- [`System.Windows.Media.PenLineJoin`](#systemwindowsmediapenlinejoin)
- [`System.Windows.Media.PixelFormatType`](#systemwindowsmediapixelformattype)
- [`System.Windows.Media.PlaneProjection`](#systemwindowsmediaplaneprojection)
- [`System.Windows.Media.PointCollection`](#systemwindowsmediapointcollection)
- [`System.Windows.Media.PolyBezierSegment`](#systemwindowsmediapolybeziersegment)
- [`System.Windows.Media.PolyLineSegment`](#systemwindowsmediapolylinesegment)
- [`System.Windows.Media.PolyQuadraticBezierSegment`](#systemwindowsmediapolyquadraticbeziersegment)
- [`System.Windows.Media.Projection`](#systemwindowsmediaprojection)
- [`System.Windows.Media.QuadraticBezierSegment`](#systemwindowsmediaquadraticbeziersegment)
- [`System.Windows.Media.RadialGradientBrush`](#systemwindowsmediaradialgradientbrush)
- [`System.Windows.Media.RateChangedRoutedEventArgs`](#systemwindowsmediaratechangedroutedeventargs)
- [`System.Windows.Media.RateChangedRoutedEventHandler`](#systemwindowsmediaratechangedroutedeventhandler)
- [`System.Windows.Media.RectangleGeometry`](#systemwindowsmediarectanglegeometry)
- [`System.Windows.Media.RenderingEventArgs`](#systemwindowsmediarenderingeventargs)
- [`System.Windows.Media.RotateTransform`](#systemwindowsmediarotatetransform)
- [`System.Windows.Media.SampleAllocationMode`](#systemwindowsmediasampleallocationmode)
- [`System.Windows.Media.ScaleTransform`](#systemwindowsmediascaletransform)
- [`System.Windows.Media.SkewTransform`](#systemwindowsmediaskewtransform)
- [`System.Windows.Media.SolidColorBrush`](#systemwindowsmediasolidcolorbrush)
- [`System.Windows.Media.Stretch`](#systemwindowsmediastretch)
- [`System.Windows.Media.StyleSimulations`](#systemwindowsmediastylesimulations)
- [`System.Windows.Media.SweepDirection`](#systemwindowsmediasweepdirection)
- [`System.Windows.Media.TextHintingMode`](#systemwindowsmediatexthintingmode)
- [`System.Windows.Media.TextOptions`](#systemwindowsmediatextoptions)
- [`System.Windows.Media.TileBrush`](#systemwindowsmediatilebrush)
- [`System.Windows.Media.TimelineMarker`](#systemwindowsmediatimelinemarker)
- [`System.Windows.Media.TimelineMarkerCollection`](#systemwindowsmediatimelinemarkercollection)
- [`System.Windows.Media.TimelineMarkerRoutedEventArgs`](#systemwindowsmediatimelinemarkerroutedeventargs)
- [`System.Windows.Media.TimelineMarkerRoutedEventHandler`](#systemwindowsmediatimelinemarkerroutedeventhandler)
- [`System.Windows.Media.Transform`](#systemwindowsmediatransform)
- [`System.Windows.Media.TransformCollection`](#systemwindowsmediatransformcollection)
- [`System.Windows.Media.TransformGroup`](#systemwindowsmediatransformgroup)
- [`System.Windows.Media.TranslateTransform`](#systemwindowsmediatranslatetransform)
- [`System.Windows.Media.Typeface`](#systemwindowsmediatypeface)
- [`System.Windows.Media.Typography`](#systemwindowsmediatypography)
- [`System.Windows.Media.VideoBrush`](#systemwindowsmediavideobrush)
- [`System.Windows.Media.VideoCaptureDevice`](#systemwindowsmediavideocapturedevice)
- [`System.Windows.Media.VideoCaptureDeviceCollection`](#systemwindowsmediavideocapturedevicecollection)
- [`System.Windows.Media.VideoFormat`](#systemwindowsmediavideoformat)
- [`System.Windows.Media.VideoOutputConnector`](#systemwindowsmediavideooutputconnector)
- [`System.Windows.Media.VideoOutputConnectorType`](#systemwindowsmediavideooutputconnectortype)
- [`System.Windows.Media.VideoSink`](#systemwindowsmediavideosink)
- [`System.Windows.Media.VisualTreeHelper`](#systemwindowsmediavisualtreehelper)
- [`System.Windows.Media.WaveFormatType`](#systemwindowsmediawaveformattype)
- [`System.Windows.MediaFailedRoutedEventArgs`](#systemwindowsmediafailedroutedeventargs)
- [`System.Windows.MessageBox`](#systemwindowsmessagebox)
- [`System.Windows.MessageBoxButton`](#systemwindowsmessageboxbutton)
- [`System.Windows.MessageBoxResult`](#systemwindowsmessageboxresult)
- [`System.Windows.Messaging.ListenFailedException`](#systemwindowsmessaginglistenfailedexception)
- [`System.Windows.Messaging.LocalMessageReceiver`](#systemwindowsmessaginglocalmessagereceiver)
- [`System.Windows.Messaging.LocalMessageSender`](#systemwindowsmessaginglocalmessagesender)
- [`System.Windows.Messaging.MessageReceivedEventArgs`](#systemwindowsmessagingmessagereceivedeventargs)
- [`System.Windows.Messaging.ReceiverNameScope`](#systemwindowsmessagingreceivernamescope)
- [`System.Windows.Messaging.SendCompletedEventArgs`](#systemwindowsmessagingsendcompletedeventargs)
- [`System.Windows.Messaging.SendFailedException`](#systemwindowsmessagingsendfailedexception)
- [`System.Windows.Navigation.LoadCompletedEventHandler`](#systemwindowsnavigationloadcompletedeventhandler)
- [`System.Windows.Navigation.NavigatedEventHandler`](#systemwindowsnavigationnavigatedeventhandler)
- [`System.Windows.Navigation.NavigatingCancelEventArgs`](#systemwindowsnavigationnavigatingcanceleventargs)
- [`System.Windows.Navigation.NavigatingCancelEventHandler`](#systemwindowsnavigationnavigatingcanceleventhandler)
- [`System.Windows.Navigation.NavigationEventArgs`](#systemwindowsnavigationnavigationeventargs)
- [`System.Windows.Navigation.NavigationMode`](#systemwindowsnavigationnavigationmode)
- [`System.Windows.NotificationWindow`](#systemwindowsnotificationwindow)
- [`System.Windows.NullableBoolConverter`](#systemwindowsnullableboolconverter)
- [`System.Windows.OutOfBrowserSettings`](#systemwindowsoutofbrowsersettings)
- [`System.Windows.Point`](#systemwindowspoint)
- [`System.Windows.PresentationFrameworkCollection`1`](#systemwindowspresentationframeworkcollection1)
- [`System.Windows.Printing.BeginPrintEventArgs`](#systemwindowsprintingbeginprinteventargs)
- [`System.Windows.Printing.EndPrintEventArgs`](#systemwindowsprintingendprinteventargs)
- [`System.Windows.Printing.PrintDocument`](#systemwindowsprintingprintdocument)
- [`System.Windows.Printing.PrintPageEventArgs`](#systemwindowsprintingprintpageeventargs)
- [`System.Windows.PropertyChangedCallback`](#systemwindowspropertychangedcallback)
- [`System.Windows.PropertyMetadata`](#systemwindowspropertymetadata)
- [`System.Windows.PropertyPath`](#systemwindowspropertypath)
- [`System.Windows.PropertyPathConverter`](#systemwindowspropertypathconverter)
- [`System.Windows.Rect`](#systemwindowsrect)
- [`System.Windows.ResourceDictionary`](#systemwindowsresourcedictionary)
- [`System.Windows.Resources.StreamResourceInfo`](#systemwindowsresourcesstreamresourceinfo)
- [`System.Windows.RoutedEvent`](#systemwindowsroutedevent)
- [`System.Windows.RoutedEventArgs`](#systemwindowsroutedeventargs)
- [`System.Windows.RoutedEventHandler`](#systemwindowsroutedeventhandler)
- [`System.Windows.RoutedPropertyChangedEventArgs`1`](#systemwindowsroutedpropertychangedeventargs1)
- [`System.Windows.RoutedPropertyChangedEventHandler`1`](#systemwindowsroutedpropertychangedeventhandler1)
- [`System.Windows.SecuritySettings`](#systemwindowssecuritysettings)
- [`System.Windows.Setter`](#systemwindowssetter)
- [`System.Windows.SetterBase`](#systemwindowssetterbase)
- [`System.Windows.SetterBaseCollection`](#systemwindowssetterbasecollection)
- [`System.Windows.Shapes.Ellipse`](#systemwindowsshapesellipse)
- [`System.Windows.Shapes.Line`](#systemwindowsshapesline)
- [`System.Windows.Shapes.Path`](#systemwindowsshapespath)
- [`System.Windows.Shapes.Polygon`](#systemwindowsshapespolygon)
- [`System.Windows.Shapes.Polyline`](#systemwindowsshapespolyline)
- [`System.Windows.Shapes.Rectangle`](#systemwindowsshapesrectangle)
- [`System.Windows.Shapes.Shape`](#systemwindowsshapesshape)
- [`System.Windows.Size`](#systemwindowssize)
- [`System.Windows.SizeChangedEventArgs`](#systemwindowssizechangedeventargs)
- [`System.Windows.SizeChangedEventHandler`](#systemwindowssizechangedeventhandler)
- [`System.Windows.StartupEventArgs`](#systemwindowsstartupeventargs)
- [`System.Windows.StartupEventHandler`](#systemwindowsstartupeventhandler)
- [`System.Windows.Style`](#systemwindowsstyle)
- [`System.Windows.StyleTypedPropertyAttribute`](#systemwindowsstyletypedpropertyattribute)
- [`System.Windows.SystemColors`](#systemwindowssystemcolors)
- [`System.Windows.SystemParameters`](#systemwindowssystemparameters)
- [`System.Windows.TemplateBindingExpression`](#systemwindowstemplatebindingexpression)
- [`System.Windows.TemplatePartAttribute`](#systemwindowstemplatepartattribute)
- [`System.Windows.TemplateVisualStateAttribute`](#systemwindowstemplatevisualstateattribute)
- [`System.Windows.TextAlignment`](#systemwindowstextalignment)
- [`System.Windows.TextDecorationCollection`](#systemwindowstextdecorationcollection)
- [`System.Windows.TextDecorations`](#systemwindowstextdecorations)
- [`System.Windows.TextTrimming`](#systemwindowstexttrimming)
- [`System.Windows.TextWrapping`](#systemwindowstextwrapping)
- [`System.Windows.Thickness`](#systemwindowsthickness)
- [`System.Windows.Threading.Dispatcher`](#systemwindowsthreadingdispatcher)
- [`System.Windows.Threading.DispatcherOperation`](#systemwindowsthreadingdispatcheroperation)
- [`System.Windows.Threading.DispatcherSynchronizationContext`](#systemwindowsthreadingdispatchersynchronizationcontext)
- [`System.Windows.Threading.DispatcherTimer`](#systemwindowsthreadingdispatchertimer)
- [`System.Windows.TriggerAction`](#systemwindowstriggeraction)
- [`System.Windows.TriggerActionCollection`](#systemwindowstriggeractioncollection)
- [`System.Windows.TriggerBase`](#systemwindowstriggerbase)
- [`System.Windows.TriggerCollection`](#systemwindowstriggercollection)
- [`System.Windows.UIElement`](#systemwindowsuielement)
- [`System.Windows.VerticalAlignment`](#systemwindowsverticalalignment)
- [`System.Windows.Visibility`](#systemwindowsvisibility)
- [`System.Windows.VisualState`](#systemwindowsvisualstate)
- [`System.Windows.VisualStateChangedEventArgs`](#systemwindowsvisualstatechangedeventargs)
- [`System.Windows.VisualStateGroup`](#systemwindowsvisualstategroup)
- [`System.Windows.VisualStateManager`](#systemwindowsvisualstatemanager)
- [`System.Windows.VisualTransition`](#systemwindowsvisualtransition)
- [`System.Windows.Window`](#systemwindowswindow)
- [`System.Windows.WindowCollection`](#systemwindowswindowcollection)
- [`System.Windows.WindowResizeEdge`](#systemwindowswindowresizeedge)
- [`System.Windows.WindowSettings`](#systemwindowswindowsettings)
- [`System.Windows.WindowStartupLocation`](#systemwindowswindowstartuplocation)
- [`System.Windows.WindowState`](#systemwindowswindowstate)
- [`System.Windows.WindowStyle`](#systemwindowswindowstyle)
- [`System.Xaml.IMarkupExtension`1`](#systemxamlimarkupextension1)
- [`System.Xaml.IRootObjectProvider`](#systemxamlirootobjectprovider)

## Microsoft.Internal.IManagedFrameworkInternalHelper

This type is not intended to be used or implemented by your code.

### Methods

| Member | Description |
|---|---|
| `SetContextEx(IntPtr)` | [SECURITY CRITICAL] This method is not intended to be used or implemented by your code. <br>`context`: Do not use. |

## Microsoft.Xna.Framework.Silverlight.GraphicsDeviceManager

Provides functionality for managing the `GraphicsDevice`. Use the static method `Current`, to get an instance the global device manager.

### Properties / fields

| Member | Description |
|---|---|
| `Current` | Static property that gets an instance of the GraphicsDeviceManger. <br>Returns: The graphics device manager. |
| `GraphicsDevice` | Gets the graphics device. <br>Returns: The graphics device. |
| `RenderMode` | Gets the current render mode. <br>Returns: The render mode. |
| `RenderModeReason` | Gets the explanation for the value of the rendering mode. <br>Returns: The reason for the current rendering mode. |

### Events

| Member | Description |
|---|---|
| `RenderModeChanged` | Occurs when the render mode changes. |

## Microsoft.Xna.Framework.Silverlight.RenderMode

Describes the types of rendering.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Hardware` | Indicates that rendering is hardware accelerated by the GPU. |
| `Unavailable` | Indicates that no renderer was available. |

## Microsoft.Xna.Framework.Silverlight.RenderModeChangedEventArgs

Provides data for the `RenderModeChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `OldRenderMode` | Gets the value of the previous rendering mode. <br>Returns: The previous render mode. |
| `OldRenderModeReason` | Gets the value of the previous explanation for the rendering mode value. <br>Returns: The previous explanation for the rendering mode value. |
| `RenderMode` | Gets the current rendering mode. <br>Returns: The current rendering mode. |
| `RenderModeReason` | Gets the current explanation for the value of the render mode. <br>Returns: The current explanation for the value of the render mode. |

## Microsoft.Xna.Framework.Silverlight.RenderModeReason

Describes the possible explanation for the `RenderMode` value.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `GPUAccelerationDisabled` | GPU hardware acceleration is disabled. |
| `Normal` | Noraml. |
| `Not3DCapable` | The graphics card is not 3D capable. |
| `SecurityBlocked` | Blocked for security reasons. |
| `TemporarilyUnavailable` | Temporarily unavailable. |

## System.Collections.ObjectModel.ObservableCollection`1

Represents a dynamic data collection that provides notifications when items get added, removed, or when the entire list is refreshed.

### Events

| Member | Description |
|---|---|
| `CollectionChanged` | Occurs when an item in the collection changes, or the entire collection changes. |
| `PropertyChanged` | Occurs when a property on an individual item in the collection changes. |
| `System#ComponentModel#INotifyPropertyChanged#PropertyChanged` | Occurs when a property value changes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new, empty instance of the `ObservableCollection`1` class. |
| `Constructor(IEnumerable{`0})` | Initializes a new instance of the `ObservableCollection`1` class and populates it with items copied from the specified collection. <br>`collection`: The collection from which the items are copied. |
| `Constructor(List{`0})` | Initializes a new instance of the `ObservableCollection`1` class and populates it with items copied from the specified list. <br>`list`: The list from which the items are copied. |
| `ClearItems` | Removes all items from the collection. |
| `InsertItem(Int32,`0)` | Inserts an item into the collection at the specified index. <br>`index`: The zero-based index at which should be inserted. <br>`item`: The object to insert. |
| `OnCollectionChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Raises the `CollectionChanged` event with the provided event data. <br>`e`: The event data to report in the event. |
| `OnPropertyChanged(ComponentModel.PropertyChangedEventArgs)` | Raises the `PropertyChanged` event with the provided arguments. <br>`e`: The event data to report in the event. |
| `RemoveItem(Int32)` | Removes the item at the specified index from the collection. <br>`index`: The zero-based index of the item to remove. |
| `SetItem(Int32,`0)` | Replaces the item at the specified index. <br>`index`: The zero-based index of the item to replace. <br>`item`: The new value for the item at the specified index. |

## System.Collections.ObjectModel.ReadOnlyObservableCollection`1

Represents a read-only `ObservableCollection`1`.

### Events

| Member | Description |
|---|---|
| `CollectionChanged` | Occurs when an item is added or removed. |
| `PropertyChanged` | Occurs when a property value changes. |
| `System#Collections#Specialized#INotifyCollectionChanged#CollectionChanged` | For a description of this member, see `CollectionChanged`. |
| `System#ComponentModel#INotifyPropertyChanged#PropertyChanged` | For a description of this member, see `PropertyChanged`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Collections.ObjectModel.ObservableCollection{`0})` | Initializes a new instance of the `ReadOnlyObservableCollection`1` class that serves as a wrapper for the specified `ObservableCollection`1`. <br>`list`: The collection to wrap. |
| `OnCollectionChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Raises the `CollectionChanged` event. <br>`args`: The event data. |
| `OnPropertyChanged(ComponentModel.PropertyChangedEventArgs)` | Raises the `PropertyChanged` event. <br>`args`: The event data. |

## System.Collections.Specialized.INotifyCollectionChanged

Notifies listeners of dynamic changes to a collection, such as when items are added and removed, or the entire collection object is reset.

### Events

| Member | Description |
|---|---|
| `CollectionChanged` | Occurs when the items list of the collection has changed, or the collection is reset. |

## System.Collections.Specialized.NotifyCollectionChangedAction

Describes the action that caused a `CollectionChanged` event.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Add` | One or more items were added to the collection. |
| `Remove` | One or more items were removed from the collection. |
| `Replace` | One or more items were replaced in the collection. |
| `Reset` | The content of the collection changed dramatically. |

## System.Collections.Specialized.NotifyCollectionChangedEventArgs

Provides event data for the `CollectionChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `Action` | Gets the description of the action that caused the event. <br>Returns: The description of the action that caused the event, as a value of the enumeration. |
| `NewItems` | Gets the items affected by an action. <br>Returns: The list of items affected by an action. The default is null. |
| `NewStartingIndex` | Gets the index at which the change occurred. <br>Returns: The index at which the change occurred. |
| `OldItems` | Gets the item affected by a `Replace` or `Remove` action. <br>Returns: The list of items affected by a `Replace` or `Remove` action. |
| `OldStartingIndex` | Gets the index at which the change occurred for a `Replace` or `Remove` action. <br>Returns: The index at which the change occurred for a `Replace` or `Remove` action. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Collections.Specialized.NotifyCollectionChangedAction)` | Initializes a new instance of the `NotifyCollectionChangedEventArgs` class, specifying the `NotifyCollectionChangedAction` value. <br>`action`: The action value, as a value of the enumeration. This should always be `Reset`; see Remarks. |
| `Constructor(Collections.Specialized.NotifyCollectionChangedAction,Object,Int32)` | Initializes a new instance of the `NotifyCollectionChangedEventArgs` class that describes an `Add` or `Remove` change. <br>`action`: The action value, as a value of the enumeration. This should be `Add` or `Remove`; see Remarks. <br>`changedItem`: The item that is affected by the change. <br>`index`: The index where the change occurred. |
| `Constructor(Collections.Specialized.NotifyCollectionChangedAction,Object,Object,Int32)` | Initializes a new instance of the `NotifyCollectionChangedEventArgs` class that describes a `Replace` change. <br>`action`: The action value, as a value of the enumeration. This should be `Replace`; see Remarks. <br>`newItem`: The new item that is replacing the original item. <br>`oldItem`: The original item that is replaced. <br>`index`: The index of the item being replaced. |

## System.Collections.Specialized.NotifyCollectionChangedEventHandler

Represents the method that handles events that implement the `CollectionChanged` event.

## System.ComponentModel.AlternateContentPropertyAttribute

Indicates whether a non-content property should be displayed in the the object tree.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AlternateContentPropertyAttribute` class. |

## System.ComponentModel.ClosingEventArgs

Provides data for the `Closing` event.

### Properties / fields

| Member | Description |
|---|---|
| `IsCancelable` | Gets a value that indicates whether you can cancel the `Closing` event. <br>Returns: true if you can cancel the event; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Boolean)` | Initializes a new instance of the `ClosingEventArgs` class. <br>`isCancelable`: Initializes the `IsCancelable` property. |

## System.ComponentModel.CurrentChangingEventArgs

Provides data for the `CurrentChanging` event.

### Properties / fields

| Member | Description |
|---|---|
| `Cancel` | Gets or sets a value that indicates whether the `CurrentItem` change should be canceled. <br>Returns: true if the event should be canceled; otherwise, false. The default is false. |
| `IsCancelable` | Gets a value that indicates whether the `CurrentItem` change can be canceled. <br>Returns: true if the event can be canceled; false if the event cannot be canceled. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CurrentChangingEventArgs` class and sets the `IsCancelable` property to true. |
| `Constructor(Boolean)` | Initializes a new instance of the `CurrentChangingEventArgs` class and sets the `IsCancelable` property to the specified value. <br>`isCancelable`: true to disable the ability to cancel a `CurrentItem` change; false to enable cancellation. |

## System.ComponentModel.CurrentChangingEventHandler

Represents a method that can handle the `CurrentChanging` event of an `ICollectionView` implementation.

## System.ComponentModel.DataErrorsChangedEventArgs

Provides data for the `ErrorsChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `PropertyName` | Gets the name of the property for which the errors changed, or null or `Empty` for entity-level errors. <br>Returns: The name of the affected property, or null or `Empty` for entity-level errors. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `DataErrorsChangedEventArgs` class. <br>`propertyName`: The name of the property for which the errors changed, or null or `Empty` for entity-level errors. |

## System.ComponentModel.DesignerProperties

Provides attached properties that can be used to communicate with a designer.

### Properties / fields

| Member | Description |
|---|---|
| `IsInDesignMode` | Gets or sets whether a `DependencyObject` is running in the context of a designer. <br>Returns: A `Boolean` that indicates whether a `DependencyObject` is running in the context of a designer. |
| `IsInDesignTool` | Gets a value that indicates whether the element is running in the context of a designer. <br>Returns: true if the element is running in the context of a designer; otherwise, false. |
| `RefreshOnlyXmlnsDefinitionsOnAssemblyReplace` | Gets or sets a value that indicates whether all the metadata associated with an assembly or just the xmlns namespace definitions should be refreshed in the designer when an assembly is recompiled. <br>Returns: true if only namespace definitions should be refreshed; otherwise, false. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsInDesignModeProperty` | Identifies the `IsInDesignMode` attached property. |

### Methods

| Member | Description |
|---|---|
| `GetIsInDesignMode(DependencyObject)` | Gets the value of the `IsInDesignMode` attached property for the specified `UIElement`. <br>`element`: The element from which the property value is read. <br>Returns: The `IsInDesignMode` property value for the element. |
| `SetIsInDesignMode(DependencyObject,Boolean)` | Sets the value of the `IsInDesignMode` attached property to a specified element. <br>`element`: The element to which the attached property is written. <br>`value`: The needed `Boolean` value. |

## System.ComponentModel.GroupDescription

Provides a base class for defining how to divide the items in a collection into groups.

### Properties / fields

| Member | Description |
|---|---|
| `GroupNames` | Gets the collection of group names. <br>Returns: The collection of group names. |

### Events

| Member | Description |
|---|---|
| `PropertyChanged` | Occurs when a property value has changed. |
| `System#ComponentModel#INotifyPropertyChanged#PropertyChanged` | For a description of this member, see `PropertyChanged`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GroupDescription` class. |
| `GroupNameFromItem(Object,Int32,Globalization.CultureInfo)` | Returns the group name or names for the specified item. <br>`item`: The item to return the group name for. <br>`level`: The level of the group within the grouping hierarchy. <br>`culture`: The culture information that affects grouping. <br>Returns: An object that represents the group name or names. |
| `NamesMatch(Object,Object)` | Indicates whether the specified item belongs in the specified group. <br>`groupName`: The name of the group to check. <br>`itemName`: The name of the item to check. <br>Returns: true if the item belongs in the group; otherwise, false. |
| `OnPropertyChanged(ComponentModel.PropertyChangedEventArgs)` | Raises the `PropertyChanged` event. <br>`e`: The event data. |
| `ShouldSerializeGroupNames` | Indicates whether the group names should be serialized. <br>Returns: true if the group names should be serialized; otherwise, false. |

## System.ComponentModel.ICollectionView

Enables collections to have the functionalities of current record management, custom sorting, filtering, and grouping.

### Properties / fields

| Member | Description |
|---|---|
| `CanFilter` | Gets a value that indicates whether this view supports filtering by way of the `Filter` property. <br>Returns: true if this view supports filtering; otherwise, false. |
| `CanGroup` | Gets a value that indicates whether this view supports grouping by way of the `GroupDescriptions` property. <br>Returns: true if this view supports grouping; otherwise, false. |
| `CanSort` | Gets a value that indicates whether this view supports sorting by way of the `SortDescriptions` property. <br>Returns: true if this view supports sorting; otherwise, false. |
| `Culture` | Gets or sets the cultural information for any operations of the view that may differ by culture, such as sorting. <br>Returns: The culture information to use during culture-sensitive operations. |
| `CurrentItem` | Gets the current item in the view. <br>Returns: The current item in the view or null if there is no current item. |
| `CurrentPosition` | Gets the ordinal position of the `CurrentItem` in the view. <br>Returns: The ordinal position of the `CurrentItem` in the view. |
| `Filter` | Gets or sets a callback that is used to determine whether an item is appropriate for inclusion in the view. <br>Returns: A method that is used to determine whether an item is appropriate for inclusion in the view. |
| `GroupDescriptions` | Gets a collection of `GroupDescription` objects that describe how the items in the collection are grouped in the view. <br>Returns: A collection of objects that describe how the items in the collection are grouped in the view. |
| `Groups` | Gets the top-level groups. <br>Returns: A read-only collection of the top-level groups or null if there are no groups. |
| `IsCurrentAfterLast` | Gets a value that indicates whether the `CurrentItem` of the view is beyond the end of the collection. <br>Returns: true if the `CurrentItem` of the view is beyond the end of the collection; otherwise, false. |
| `IsCurrentBeforeFirst` | Gets a value that indicates whether the `CurrentItem` of the view is beyond the start of the collection. <br>Returns: true if the `CurrentItem` of the view is beyond the start of the collection; otherwise, false. |
| `IsEmpty` | Gets a value that indicates whether the view is empty. <br>Returns: true if the view is empty; otherwise, false. |
| `SortDescriptions` | Gets a collection of `SortDescription` instances that describe how the items in the collection are sorted in the view. <br>Returns: A collection of values that describe how the items in the collection are sorted in the view. |
| `SourceCollection` | Gets the underlying collection. <br>Returns: The underlying collection. |

### Events

| Member | Description |
|---|---|
| `CurrentChanged` | Occurs after the current item has been changed. |
| `CurrentChanging` | Occurs before the current item changes. |

### Methods

| Member | Description |
|---|---|
| `Contains(Object)` | Indicates whether the specified item belongs to this collection view. <br>`item`: The object to check. <br>Returns: true if the item belongs to this collection view; otherwise, false. |
| `DeferRefresh` | Enters a defer cycle that you can use to merge changes to the view and delay automatic refresh. <br>Returns: The typical usage is to create a using scope with an implementation of this method and then include multiple view-changing calls within the scope. The implementation should delay automatic refresh until after the using scope exits. |
| `MoveCurrentTo(Object)` | Sets the specified item in the view as the `CurrentItem`. <br>`item`: The item to set as the current item. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `MoveCurrentToFirst` | Sets the first item in the view as the `CurrentItem`. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `MoveCurrentToLast` | Sets the last item in the view as the `CurrentItem`. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `MoveCurrentToNext` | Sets the item after the `CurrentItem` in the view as the `CurrentItem`. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `MoveCurrentToPosition(Int32)` | Sets the item at the specified index to be the `CurrentItem` in the view. <br>`position`: The index to set the `CurrentItem` to. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `MoveCurrentToPrevious` | Sets the item before the `CurrentItem` in the view to the `CurrentItem`. <br>Returns: true if the resulting `CurrentItem` is an item in the view; otherwise, false. |
| `Refresh` | Recreates the view. |

## System.ComponentModel.ICollectionViewFactory

Defines a method that enables a collection to provide a custom view for specialized sorting, filtering, grouping, and currency.

### Methods

| Member | Description |
|---|---|
| `CreateView` | Returns a custom view for specialized sorting, filtering, grouping, and currency. <br>Returns: A custom view for specialized sorting, filtering, grouping, and currency. |

## System.ComponentModel.IEditableCollectionView

Defines methods and properties that a collection view implements to provide editing capabilities to a collection.

### Properties / fields

| Member | Description |
|---|---|
| `CanAddNew` | Gets a value that indicates whether a new item can be added to the collection. <br>Returns: true if a new item can be added to the collection; otherwise, false. |
| `CanCancelEdit` | Gets a value that indicates whether the collection view can discard pending changes and restore the original values of an edited object. <br>Returns: true if the collection view can discard pending changes and restore the original values of an edited object; otherwise, false. |
| `CanRemove` | Gets a value that indicates whether an item can be removed from the collection. <br>Returns: true if an item can be removed from the collection; otherwise, false. |
| `CurrentAddItem` | Gets the item that is being added during the current add transaction. <br>Returns: The item that is being added if `IsAddingNew` is true; otherwise, null. |
| `CurrentEditItem` | Gets the item in the collection that is being edited. <br>Returns: The item that is being edited if `IsEditingItem` is true; otherwise, null. |
| `IsAddingNew` | Gets a value that indicates whether an add transaction is in progress. <br>Returns: true if an add transaction is in progress; otherwise, false. |
| `IsEditingItem` | Gets a value that indicates whether an edit transaction is in progress. <br>Returns: true if an edit transaction is in progress; otherwise, false. |
| `NewItemPlaceholderPosition` | Gets or sets the position of the new item placeholder in the collection view. <br>Returns: An enumeration value that specifies the position of the new item placeholder in the collection view. |

### Methods

| Member | Description |
|---|---|
| `AddNew` | Adds a new item to the underlying collection. <br>Returns: The new item that is added to the collection. |
| `CancelEdit` | Ends the edit transaction and, if possible, restores the original value of the item. |
| `CancelNew` | Ends the add transaction and discards the pending new item. |
| `CommitEdit` | Ends the edit transaction and saves the pending changes. |
| `CommitNew` | Ends the add transaction and saves the pending new item. |
| `EditItem(Object)` | Begins an edit transaction on the specified item. <br>`item`: The item to edit. |
| `Remove(Object)` | Removes the specified item from the collection. <br>`item`: The item to remove. |
| `RemoveAt(Int32)` | Removes the item at the specified position from the collection. <br>`index`: Index of item to remove. |

## System.ComponentModel.INotifyDataErrorInfo

Defines members that data entity classes can implement to provide custom synchronous and asynchronous validation support.

### Properties / fields

| Member | Description |
|---|---|
| `HasErrors` | Gets a value that indicates whether the entity has validation errors. <br>Returns: true if the entity currently has validation errors; otherwise, false. |

### Events

| Member | Description |
|---|---|
| `ErrorsChanged` | Occurs when the validation errors have changed for a property or for the entire entity. |

### Methods

| Member | Description |
|---|---|
| `GetErrors(String)` | Gets the validation errors for a specified property or for the entire entity. <br>`propertyName`: The name of the property to retrieve validation errors for; or null or `Empty`, to retrieve entity-level errors. <br>Returns: The validation errors for the property or entity. |

## System.ComponentModel.NewItemPlaceholderPosition

Specifies where the placeholder for a new item appears in an `IEditableCollectionView`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AtEnd` | The placeholder for a new item appears at the end of the collection. New items are added at the end of the collection, before the new item placeholder. |
| `None` | The collection does not use a new item placeholder. The position of items that are added depends on the underlying collection. Usually, they are added at the end of the collection. |

## System.ComponentModel.SortDescription

Defines the direction and the property name that will be used as the criteria for sorting a collection.

### Properties / fields

| Member | Description |
|---|---|
| `Direction` | Gets or sets a value that indicates whether to sort in ascending or descending order. <br>Returns: A value that indicates the sort direction. |
| `IsSealed` | Gets a value that indicates whether this structure is in an immutable state. <br>Returns: true if this object is being used; otherwise, false. |
| `PropertyName` | Gets or sets the property name being used as the sorting criteria. <br>Returns: The name of the property to sort by. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String,ComponentModel.ListSortDirection)` | Initializes a new instance of the `SortDescription` structure. <br>`propertyName`: The name of the property to sort the list by. <br>`direction`: The sort order. |
| `Equals(Object)` | Compares the specified instance and the current instance of `SortDescription` for value equality. <br>`obj`: The `SortDescription` instance to compare. <br>Returns: true if `obj` and this `SortDescription` instance have the same `PropertyName` and `Direction` values; otherwise, false. |
| `GetHashCode` | Returns the hash code for the current instance. <br>Returns: The hash code for the current instance. |
| `op_Equality(ComponentModel.SortDescription,ComponentModel.SortDescription)` | Compares two `SortDescription` instances for value equality. <br>`sd1`: The first `SortDescription` instance to compare. <br>`sd2`: The second `SortDescription` instance to compare. <br>Returns: true if the two `SortDescription` instances have the same `PropertyName` and `Direction` values; otherwise, false. |
| `op_Inequality(ComponentModel.SortDescription,ComponentModel.SortDescription)` | Compares two `SortDescription` instances for value inequality. <br>`sd1`: The first `SortDescription` instance to compare. <br>`sd2`: The second `SortDescription` instance to compare. <br>Returns: true if the two `SortDescription` instances do not have the same `PropertyName` and `Direction` values; otherwise, false. |

## System.ComponentModel.SortDescriptionCollection

Represents a collection of `SortDescription` instances.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Empty` | Gets an empty and non-modifiable `SortDescriptionCollection`. |

### Events

| Member | Description |
|---|---|
| `CollectionChanged` | Occurs when a `SortDescription` is added or removed. |
| `System#Collections#Specialized#INotifyCollectionChanged#CollectionChanged` | For a description of this member, see `CollectionChanged`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SortDescriptionCollection` class. |
| `ClearItems` | Removes all `SortDescription` instances from the collection. |
| `InsertItem(Int32,ComponentModel.SortDescription)` | Inserts a `SortDescription` into the collection at the specified index. <br>`index`: The zero-based index where the `SortDescription` is inserted. <br>`item`: The `SortDescription` to insert. |
| `RemoveItem(Int32)` | Removes the `SortDescription` at the specified index in the collection. <br>`index`: The zero-based index of the `SortDescription` to remove. |
| `SetItem(Int32,ComponentModel.SortDescription)` | Replaces the `SortDescription` at the specified index. <br>`index`: The zero-based index of the `SortDescription` to replace. <br>`item`: The new value for the `SortDescription` at the specified index. |

## System.IO.IsolatedStorage.IsolatedStorageSettings

Provides a `Dictionary`2` that stores key-value pairs in isolated storage.

### Properties / fields

| Member | Description |
|---|---|
| `ApplicationSettings` | Gets an instance of `IsolatedStorageSettings` that contains the contents of the application's `IsolatedStorageFile`, scoped at the application level, or creates a new instance of `IsolatedStorageSettings` if one does not exist. <br>Returns: An `IsolatedStorageSettings` object that contains the contents of the application's `IsolatedStorageFile`, scoped at the application level. If an instance does not already exist, a new instance is created. |
| `Count` | Gets the number of key-value pairs that are stored in the dictionary. <br>Returns: The number of key-value pairs that are stored in the dictionary. |
| `Item` | Gets or sets the value associated with the specified key. <br>`key`: The key of the item to get or set. <br>Returns: The value associated with the specified key. If the specified key is not found, a get operation throws a `ZeyNotFoundException`, and a set operation creates a new element that has the specified key. |
| `Zeys` | Gets a collection that contains the keys in the dictionary. <br>Returns: A `ZeyCollection` that contains the keys in the `Dictionary`2`. |
| `SiteSettings` | Gets an instance of `IsolatedStorageSettings` that contains the contents of the application's `IsolatedStorageFile`, scoped at the domain level, or creates a new instance of `IsolatedStorageSettings` if one does not exist. <br>Returns: An `IsolatedStorageSettings` object that contains the contents of the application's `IsolatedStorageFile`, scoped at the domain level. If an instance does not already exist, a new instance is created. |
| `System#Collections#Generic#ICollection{T}#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the `IDictionary`2` is read-only; otherwise, false. In the default implementation of `Dictionary`2`, this property always returns false. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Zeys` | For a description of this member, see `Zeys`. <br>Returns: An `ICollection`1` that contains the keys of the object that implements `IDictionary`2`. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Values` | For a description of this member, see `Values`. <br>Returns: An `ICollection`1` that contains the values in the object that implements `IDictionary`2`. |
| `System#Collections#ICollection#Count` | For a description of this member, see `Count`. <br>Returns: The number of elements that are contained in the `ICollection`. |
| `System#Collections#ICollection#IsSynchronized` | For a description of this member, see `IsSynchronized`. <br>Returns: true if access to the `ICollection` is synchronized (thread safe); otherwise, false. In the default implementation of `Dictionary`2`, this property always returns false. |
| `System#Collections#ICollection#SyncRoot` | For a description of this member, see `SyncRoot`. <br>Returns: An object that can be used to synchronize access to the `ICollection`. |
| `System#Collections#IDictionary#IsFixedSize` | For a description of this member, see `IsFixedSize`. <br>Returns: true if the `IDictionary` has a fixed size; otherwise, false. In the default implementation of `IDictionary`, this property always returns false. |
| `System#Collections#IDictionary#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the `IDictionary` is read-only; otherwise, false. In the default implementation of `IDictionary`, this property always returns false. |
| `System#Collections#IDictionary#Item` | For a description of this member, see `Object)`. <br>`key`: The key of the value to get or set. <br>Returns: The value associated with the specified . |
| `Values` | Gets a collection that contains the values in the dictionary. <br>Returns: A `ValueCollection` that contains the values in the `Dictionary`2`. |

### Methods

| Member | Description |
|---|---|
| `Add(String,Object)` | Adds an entry to the dictionary for the key-value pair. <br>`key`: The key for the entry to be stored. <br>`value`: The value to be stored. |
| `Clear` | Resets the count of items stored in `IsolatedStorageSettings` to zero and releases all references to elements in the collection. |
| `Contains(String)` | Determines if the application settings dictionary contains the specified key. <br>`key`: The key for the entry to be located. <br>Returns: true if the dictionary contains the specified key; otherwise, false. |
| `Remove(String)` | Removes the entry with the specified key. <br>`key`: The key for the entry to be deleted. <br>Returns: true if the specified key was removed; otherwise, false. |
| `Save` | Saves data written to the current `IsolatedStorageSettings` object. |
| `System#Collections#Generic#ICollection{T}#Add(ZeyValuePair{String,Object})` | For a description of this member, see `Add(`0)`. <br>`item`: The `ZeyValuePair`2` to be added. |
| `System#Collections#Generic#ICollection{T}#Clear` | For a description of this member, see `Clear`. |
| `System#Collections#Generic#ICollection{T}#Contains(ZeyValuePair{String,Object})` | For a description of this member, see `Contains(`0)`. <br>`item`: The key-value pair for the entry to be located. <br>Returns: true if `item` is found in the `ICollection`1`; otherwise, false. |
| `System#Collections#Generic#ICollection{T}#CopyTo(ZeyValuePair{String,Object}[],Int32)` | For a description of this member, see `Int32)`. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from `ICollection`1`. The array must have zero-based indexing. <br>`arrayIndex`: The zero-based index in at which copying begins. |
| `System#Collections#Generic#ICollection{T}#Remove(ZeyValuePair{String,Object})` | For a description of this member, see `Remove(`0)`. <br>`item`: The key for the entry to be deleted. <br>Returns: true if `item` was successfully removed from the `ICollection`1`; otherwise, false. This method also returns false if `item` is not found in the original `ICollection`1`. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#ContainsKey(String)` | For a description of this member, see `ContainsKey(`0)`. <br>`key`: The key for the entry to be located. <br>Returns: true if the `IDictionary`2` contains an element with the specified `key`; otherwise, false. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#TryGetValue(String,Object@)` | For a description of this member, see `TryGetValue(`0,`1@)`. <br>`key`: The key of the value to get. <br>`value`: When this method returns, the value associated with the specified key if the key is found; otherwise, the default value for the type of the parameter. This parameter is passed uninitialized. <br>Returns: true if the `IDictionary`2` contains an element with the specified `key`; otherwise, false. |
| `System#Collections#Generic#IEnumerable{T}#GetEnumerator` | For a description of this member, see `GetEnumerator`. <br>Returns: An `IEnumerator`1` object that can be used to iterate through the collection. |
| `System#Collections#ICollection#CopyTo(Array,Int32)` | For a description of this member, see `Int32)`. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from `ICollection`. The array must have zero-based indexing. <br>`index`: The zero-based index in at which copying begins. |
| `System#Collections#IDictionary#Add(Object,Object)` | For a description of this member, see `Object)`. <br>`key`: The object to use as the key of the element to add. <br>`value`: The object to use as the value of the element to add. |
| `System#Collections#IDictionary#Clear` | For a description of this member, see `Clear`. |
| `System#Collections#IDictionary#Contains(Object)` | For a description of this member, see `Object)`. <br>`key`: The key to locate in the `IDictionary`. <br>Returns: true if the `IDictionary` contains an element with the specified `key`; otherwise, false. |
| `System#Collections#IDictionary#GetEnumerator` | For a description of this member, see `GetEnumerator`. <br>Returns: An `IEnumerator`1` object that can be used to iterate through the collection. |
| `System#Collections#IDictionary#Remove(Object)` | For a description of this member, see `Object)`. <br>`key`: The key for the entry to be deleted. |
| `System#Collections#IEnumerable#GetEnumerator` | For a description of this member, see `GetEnumerator`. <br>Returns: An `IEnumerator`1` object that can be used to iterate through the collection. |
| `TryGetValue``1(String,``0@)` | Gets a value for the specified key. <br>`key`: The key of the value to get. <br>`value`: When this method returns, the value associated with the specified key if the key is found; otherwise, the default value for the type of the parameter. This parameter is passed uninitialized. <br>Returns: true if the specified key is found; otherwise, false. |

## System.Net.Browser.WebRequestCreator

Provides objects for specifying whether the browser or the client handles HTTP requests and responses.

### Properties / fields

| Member | Description |
|---|---|
| `BrowserHttp` | Gets an object that specifies browser HTTP handling for Web requests and responses. <br>Returns: An `IWebRequestCreate` object that specifies browser handling for Web requests and responses. |
| `ClientHttp` | Gets an object that specifies client HTTP handling for Web requests and responses. <br>Returns: An `IWebRequestCreate` object for use with Silverlight-based application that runs outside of a Web browser. |

## System.Net.HttpUtility

Provides methods for encoding and decoding HTML and URL strings.

### Methods

| Member | Description |
|---|---|
| `HtmlDecode(String)` | Converts a string that has been HTML-encoded (for HTTP transmission) into a decoded string. <br>`html`: An HTML-encoded string to decode. <br>Returns: A decoded string. |
| `HtmlEncode(String)` | Converts a text string into an HTML-encoded string. <br>`html`: The text to HTML-encode. <br>Returns: An HTML-encoded string. |
| `UrlDecode(String)` | Converts a string that has been encoded for transmission in a URL into a decoded string. <br>`url`: A URL-encoded string to decode. <br>Returns: A decoded string. |
| `UrlEncode(String)` | Converts a text string into a URL-encoded string. <br>`url`: The text to URL-encode. <br>Returns: A URL-encoded string. |

## System.Reflection.ICustomTypeProvider

Defines a method that returns a custom `Type` object used by the data binding system to determine the run-time structure of the implementing type.

### Methods

| Member | Description |
|---|---|
| `GetCustomType` | Returns a `Type` object that represents the run-time structure of the implementing type. <br>Returns: The run-time type of the object for purposes of data binding. |

## System.Runtime.InteropServices.Automation.AutomationEvent

Represents an Automation event.

### Events

| Member | Description |
|---|---|
| `EventRaised` | Occurs when the Automation event occurs. |

### Methods

| Member | Description |
|---|---|
| `AddEventHandler(Delegate)` | Attaches the specified delegate to the Automation event. <br>`handler`: The delegate to attach to the event. |
| `RemoveEventHandler(Delegate)` | Detaches the specified delegate from the Automation event. <br>`handler`: The delegate to detach from the event. |

## System.Runtime.InteropServices.Automation.AutomationEventArgs

Provides data for the `EventRaised` event.

### Properties / fields

| Member | Description |
|---|---|
| `Arguments` | Gets the event arguments from the Automation event. <br>Returns: The event arguments from the Automation event. |

## System.Runtime.InteropServices.Automation.AutomationFactory

Provides access to registered Automation servers.

### Properties / fields

| Member | Description |
|---|---|
| `IsAvailable` | Gets a value that indicates whether the Automation feature in Silverlight is available to the application. <br>Returns: true if the Automation feature in Silverlight is available to the application; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `CreateObject(String)` | Activates and returns a reference to the registered Automation server with the specified programmatic identifier (ProgID). <br>`progID`: The ProgID of the registered automation server to activate. <br>Returns: A late-bound reference to the specified automation server. |
| `CreateObject``1` | Not yet implemented. <br>Returns: null. |
| `GetEvent(Object,String)` | Gets an object that represents the specified event of the specified Automation server. <br>`automationObject`: A reference to the Automation server to retrieve an event for. <br>`eventName`: The name of the event to retrieve. <br>Returns: An object that represents the specified event. |
| `GetObject(String)` | Gets a reference to the previously activated and currently running Automation server with the specified programmatic identifier (ProgID). <br>`progID`: The ProgID of the registered Automation server to retrieve a reference to. <br>Returns: A late-bound reference to the specified Automation server. |

## System.Windows.Analytics

Exposes read-only data about how an application is performing.

### Properties / fields

| Member | Description |
|---|---|
| `AverageProcessLoad` | Gets a factor that reports the total load on the CPU that this process is using, determined across all cores averaged together. <br>Returns: A value between 0 and 1 that reports CPU load associated with this process, with the factor determined by examining all cores of a multi-core system averaged together. 0 maps to 0% load while 1 maps to 100% load. |
| `AverageProcessorLoad` | Gets a factor that reports the total load on the CPU by all processes, determined across all cores averaged together. <br>Returns: A value between 0 and 1 that reports CPU load by all processes, with the factor determined by examining all cores of a multi-core system averaged together. 0 maps to 0% load while 1 maps to 100% load. |
| `ClientInformation` | Gets a collection of hashed pseudo unique Base64 string values on a per domain and machine basis. <br>Returns: Returns a `ReadOnlyCollection`1` which contains the unique string values. |
| `GpuCollection` | Gets a collection of `GpuInformation` objects which each include details taken from a video driver. <br>Returns: A collection of `GpuInformation` objects which each include details taken from a video driver. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Analytics` class. |
| `Finalize` | Frees resources used by the `Analytics` class. |

## System.Windows.Application

Encapsulates a Silverlight application.

### Properties / fields

| Member | Description |
|---|---|
| `ApplicationLifetimeObjects` | Gets the application extension services that have been registered for this application. <br>Returns: The registered services. |
| `Current` | Gets the `Application` object for the current application. <br>Returns: The `Application` object for the current application. |
| `HasElevatedPermissions` | Gets a value that indicates whether the application is running with elevated permissions. <br>Returns: true if the application is running with elevated permissions; otherwise, false. |
| `Host` | Gets various details about the Silverlight application's host. <br>Returns: A `SilverlightHost` that provides various details about the Silverlight host application. |
| `InstallState` | Gets the current out-of-browser installation state of the application. <br>Returns: The out-of-browser installation state of the application. |
| `IsRunningOutOfBrowser` | Gets a value that indicates whether the application was launched from the out-of-browser state. <br>Returns: true if the application was launched from the out-of-browser state; false if the application was launched from within its host Web page. |
| `MainWindow` | Gets the out-of-browser application window. <br>Returns: The out-of-browser application window. |
| `Resources` | Gets a collection of application-scoped resources, such as styles, templates, and brushes. <br>Returns: A `ResourceDictionary` object that contains zero or more application-scoped resources. |
| `RootVisual` | Gets or sets the main application UI. <br>Returns: A `UIElement` that is the main application UI. |
| `Windows` | Gets a collection of the `Window` instances that have been created. <br>Returns: A collection of the windows used by the application. |

### Events

| Member | Description |
|---|---|
| `CheckAndDownloadUpdateCompleted` | Occurs when the application has finished checking for updates in response to a `CheckAndDownloadUpdateAsync` method call. |
| `Exit` | Occurs just before an application shuts down and cannot be canceled. |
| `InstallStateChanged` | Occurs when the `InstallState` property value changes. |
| `Startup` | Occurs when an application is started. |
| `UnhandledException` | Occurs when an exception that is raised by Silverlight is not handled. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Application` class. |
| `CheckAndDownloadUpdateAsync` | Launches an asynchronous process to check for and download an updated version of the application. |
| `GetResourceStream(Uri)` | Returns a resource file from a location in the application package. <br>`uriResource`: A relative URI that identifies the resource file to be loaded. The URI is relative to the application package and does not need a leading forward slash. <br>Returns: A `StreamResourceInfo` that contains the stream for the desired resource file. |
| `GetResourceStream(Resources.StreamResourceInfo,Uri)` | Returns a resource file from a location in the specified zip package. <br>`zipPackageStreamResourceInfo`: A `StreamResourceInfo` that contains the zip package stream with the desired resource file. <br>`uriResource`: A relative URI that identifies the resource file to be extracted from the zip package. The URI is relative to the application package and does not need a leading forward slash. <br>Returns: A `StreamResourceInfo` that contains the stream for the desired resource file. |
| `Install` | Attempts to install the application so that it can run outside the browser. <br>Returns: true if the user gave consent to install the application or the application requires elevated trust; otherwise, false. See Remarks for more information. |
| `LoadComponent(Object,Uri)` | Loads a XAML file that is located at the specified Uniform Resource Identifier (URI) and converts it to an instance of the object that is specified by the root element of the XAML file. <br>`component`: An object of the same type as the root element of the XAML file to be loaded. <br>`resourceLocator`: A relative URI that identifies the XAML file to be loaded. |

## System.Windows.ApplicationServiceContext

Represents the initial state of a Silverlight application when application extension services are started.

### Properties / fields

| Member | Description |
|---|---|
| `ApplicationInitParams` | Gets the initialization parameters specified by the host Web page when embedding the Silverlight plug-in. <br>Returns: The initialization parameters as a dictionary of key/value pairs. |

## System.Windows.ApplicationUnhandledExceptionEventArgs

Provides data for the `UnhandledException` event.

### Properties / fields

| Member | Description |
|---|---|
| `ExceptionObject` | Gets or sets the unhandled exception. <br>Returns: The unhandled exception. |
| `Handled` | Gets or sets a value that indicates whether the exception is handled. <br>Returns: true to mark the exception as handled, which indicates that Silverlight should not process it further; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Exception,Boolean)` | Initializes a new instance of the `ApplicationUnhandledExceptionEventArgs` class. <br>`ex`: The exception that is being thrown as unhandled. <br>`handled`: A value that indicates whether the exception has been handled and should not be processed further. |

## System.Windows.AssemblyPart

An assembly part is an assembly that is to be included in a Silverlight-based application package (.xap).

### Properties / fields

| Member | Description |
|---|---|
| `Source` | Gets the `Uri` that identifies an assembly as an assembly part. <br>Returns: A `String` that is the assembly, which is identified as an assembly part. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AssemblyPart` class. |
| `Load(IO.Stream)` | Converts a `Stream` to an `Assembly` that is subsequently loaded into the current application domain. <br>`assemblyStream`: The `Stream` to load into the current application domain. <br>Returns: The `Assembly` that is subsequently loaded into the current application domain. |

## System.Windows.AssemblyPartCollection

Stores a collection of `AssemblyPart` objects. Provides collection support for the `Parts` property.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AssemblyPartCollection` class. |

## System.Windows.Automation.AutomationElementIdentifiers

Contains values used as automation property identifiers by UI automation providers and UI automation clients.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AcceleratorKeyProperty` | Identifies the accelerator key automation property. The accelerator key property value is returned by the `GetAcceleratorKey` method. <br>Returns: The automation property identifier. |
| `AccessKeyProperty` | Identifies the access key automation property. The access key property value is returned by the `GetAccessKey` method. <br>Returns: The automation property identifier. |
| `AutomationIdProperty` | Identifies the automation element identifier automation property. The automation element identifier value is returned by the `GetAutomationId` method. <br>Returns: The automation property identifier. |
| `BoundingRectangleProperty` | Identifies the bounding rectangle automation property. The bounding rectangle property value is returned by the `GetBoundingRectangle` method. <br>Returns: The automation property identifier. |
| `ClassNameProperty` | Identifies the class name automation property. The class name property value is returned by the `GetClassName` method. <br>Returns: The automation property identifier. |
| `ClickablePointProperty` | Identifies the clickable point automation property. A valid clickable point property value is returned by the `GetClickablePoint` method. <br>Returns: The automation property identifier. |
| `ControlTypeProperty` | Identifies the control type automation property. The control type property value is returned by the `GetAutomationControlType` method. <br>Returns: The automation property identifier. |
| `HasKeyboardFocusProperty` | Identifies the keyboard focus automation property. The keyboard focus state is returned by the `HasKeyboardFocus` method. <br>Returns: The automation property identifier. |
| `HelpTextProperty` | Identifies the help text automation property. The help text property value is returned by the `GetHelpText` method. <br>Returns: The automation property identifier. |
| `IsContentElementProperty` | Identifies the content element determination automation property. The content element status indicates whether the element contains content that is valuable to the end user. The current status is returned by the `IsContentElement` method. <br>Returns: The automation property identifier. |
| `IsControlElementProperty` | Identifies the control element determination automation property. The control element status indicates whether the element contains user interface components that can be manipulated. The current status is returned by the `IsControlElement` method. <br>Returns: The automation property identifier. |
| `IsEnabledProperty` | Identifies the enabled determination automation property. The enabled status indicates whether the item referenced by the automation peer is enabled. The current status is returned by the `IsEnabled` method. <br>Returns: The automation property identifier. |
| `IsKeyboardFocusableProperty` | Identifies the keyboard-focusable determination automation property. The keyboard focusable status is returned by the `IsKeyboardFocusable` method. <br>Returns: The automation property identifier. |
| `IsOffscreenProperty` | Identifies the offscreen determination automation property. The offscreen status indicates whether the item referenced by the automation peer is off the screen. The current status is returned by the `IsOffscreen` method. <br>Returns: The automation property identifier. |
| `IsPasswordProperty` | Identifies the password determination automation property. The password status indicates whether the item referenced by the automation peer contains a password. The current status is returned by the `IsPassword` method. <br>Returns: The automation property identifier. |
| `IsRequiredForFormProperty` | Identifies the form requirement determination automation property. The form requirement status indicates whether the element must be completed on a form. The current status is returned by the `IsRequiredForForm` method. <br>Returns: The automation property identifier. |
| `ItemStatusProperty` | Identifies the item status automation property. The current item status is returned by the `GetItemStatus` method. <br>Returns: The automation property identifier. |
| `ItemTypeProperty` | Identifies the item type automation property. The item type value is returned by `GetItemType` method. <br>Returns: The automation property identifier. |
| `LabeledByProperty` | Identifies the labeled-by peer automation property. The current label peer is returned by the `GetLabeledBy` method. <br>Returns: The automation property identifier. |
| `LocalizedControlTypeProperty` | Identifies the localized control type automation property. The current localized control type is returned by the `GetLocalizedControlTypeCore` method. <br>Returns: The automation property identifier. |
| `NameProperty` | Identifies the element name automation property. The current name is returned by the `GetName` method. <br>Returns: The automation property identifier. |
| `OrientationProperty` | Identifies the orientation automation property. The current orientation value is returned by the `GetOrientation` method. <br>Returns: The automation property identifier. |

## System.Windows.Automation.AutomationProperties

Provides support for getting or setting the value of instance-level values of automation properties. These property values are set as attached properties (typically in XAML) and supplement or override automation property values from a control's `AutomationPeer`.

### Properties / fields

| Member | Description |
|---|---|
| `AcceleratorKey` | Gets or sets the accelerator key string for the element. <br>Returns: The accelerator key string for the element. |
| `AccessKey` | Gets or sets the access key for the element. <br>Returns: The access key for the element. |
| `AutomationId` | Gets or set the string that uniquely identifies the element to UI automation. <br>Returns: The string that uniquely identifies the element to UI automation. |
| `HelpText` | Gets or sets the help text for the element. <br>Returns: The help text for the specified element. |
| `IsRequiredForForm` | Gets or sets a value that indicates whether the element is required to be filled out on a form. <br>Returns: true if the element is required to be filled out on a form; otherwise, false. |
| `ItemStatus` | Gets or sets a description of the status of an item in an element. <br>Returns: The status of an item in an element. |
| `ItemType` | Gets or sets a description of the type of the specified element. <br>Returns: The description of the type of the specified element. |
| `LabeledBy` | Gets or sets the element that contains the text label for the element. <br>Returns: The element that contains the text label for the element. |
| `Name` | Gets or sets the automation name of the element. <br>Returns: The name of the element. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AcceleratorKeyProperty` | Identifies the `AcceleratorKey` attached property. <br>Returns: The identifier for the `AcceleratorKey` attached property. |
| `AccessKeyProperty` | Identifies the `AccessKey` attached property. <br>Returns: The identifier for the `AccessKey` attached property. |
| `AutomationIdProperty` | Identifies the `AutomationId` attached property. <br>Returns: The identifier for the `AutomationId` attached property. |
| `HelpTextProperty` | Identifies the `HelpText` attached property. <br>Returns: The identifier for the `HelpText` attached property. |
| `IsRequiredForFormProperty` | Identifies the `IsRequiredForForm` attached property. <br>Returns: The identifier for the `IsRequiredForForm` attached property. |
| `ItemStatusProperty` | Identifies the `ItemStatus` attached property. <br>Returns: The identifier for the `ItemStatus` attached property. |
| `ItemTypeProperty` | Identifies the `ItemType` attached property. <br>Returns: The identifier for the `ItemType` dependency property. |
| `LabeledByProperty` | Identifies the `LabeledBy` attached property. <br>Returns: The identifier for the `LabeledBy` attached property. |
| `NameProperty` | Identifies the `Name` attached property. <br>Returns: The identifier for the `Name` attached property. |

### Methods

| Member | Description |
|---|---|
| `GetAcceleratorKey(DependencyObject)` | Gets the value of the `AcceleratorKey` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The accelerator key, as a string. |
| `GetAccessKey(DependencyObject)` | Gets the value of the `AccessKey` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The access key, as a string. |
| `GetAutomationId(DependencyObject)` | Gets the value of the `AutomationId` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The UI Automation identifier for the specified element. |
| `GetHelpText(DependencyObject)` | Gets the value of the `HelpText` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The help text for the specified element. |
| `GetIsRequiredForForm(DependencyObject)` | Gets the value of the `IsRequiredForForm` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: true if the specified element is required for completion of a form; otherwise, false. |
| `GetItemStatus(DependencyObject)` | Gets the value of the `ItemStatus` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The item status of the element. |
| `GetItemType(DependencyObject)` | Gets the value of the `ItemType` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The item type of the element. |
| `GetLabeledBy(DependencyObject)` | Gets the value of the `LabeledBy` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The element that is targeted by the label. |
| `GetName(DependencyObject)` | Gets the value of the `Name` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` to check. <br>Returns: The name of the specified element. |
| `SetAcceleratorKey(DependencyObject,String)` | Sets the value of the `AcceleratorKey` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The accelerator key value to set. |
| `SetAccessKey(DependencyObject,String)` | Sets the value of the `AccessKey` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The access key value to set. |
| `SetAutomationId(DependencyObject,String)` | Sets the value of the `AutomationId` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The UI automation identifier value to set. |
| `SetHelpText(DependencyObject,String)` | Sets the value of the `HelpText` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The help text. |
| `SetIsRequiredForForm(DependencyObject,Boolean)` | Sets the value of the `IsRequiredForForm` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: true to specify that the element is required to be filled out on a form; otherwise, false. |
| `SetItemStatus(DependencyObject,String)` | Sets the value of the `ItemStatus` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The item status. |
| `SetItemType(DependencyObject,String)` | Sets the value of the `ItemType` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The item type. |
| `SetLabeledBy(DependencyObject,UIElement)` | Sets the value of the `LabeledBy` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The UI element that represents the label for . |
| `SetName(DependencyObject,String)` | Sets the value of the `Name` attached property for the specified `DependencyObject`. <br>`element`: The `DependencyObject` for which to set the property. <br>`value`: The object name. |

## System.Windows.Automation.AutomationProperty

Identifies a property of `AutomationElementIdentifiers`.

## System.Windows.Automation.AutomationTextAttribute

Identifies UI Automation text attributes.

## System.Windows.Automation.DockPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IDockProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DockPositionProperty` | Identifies the `DockPosition` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.DockPosition

Contains values that specify the dock position of an object within a docking container.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Bottom` | Indicates that the UI automation element is docked along the bottom edge of the docking container. |
| `Fill` | Indicates that the UI automation element is docked along all edges of the docking container and fills all available space within the container. |
| `Left` | Indicates that the UI automation element is docked along the left edge of the docking container. |
| `None` | Indicates that the UI automation element is not docked to any edge of the docking container |
| `Right` | Indicates that the UI automation element is docked along the right edge of the docking container. |
| `Top` | Indicates that the UI automation element is docked along the top edge of the docking container. |

## System.Windows.Automation.ElementNotAvailableException

The exception that is thrown when an attempt is made to access a UI automation element corresponding to a part of the user interface that is no longer available.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ElementNotAvailableException` class. |

## System.Windows.Automation.ElementNotEnabledException

The exception that is thrown when an attempt is made through UI automation to manipulate a control that is not enabled.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ElementNotEnabledException` class. |

## System.Windows.Automation.ExpandCollapsePatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IExpandCollapseProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ExpandCollapseStateProperty` | Identifies the `ExpandCollapseState` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.ExpandCollapseState

Contains values that specify the `ExpandCollapseState` automation property value of a UI automation element.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Collapsed` | No child nodes, controls, or content of the UI automation element are displayed. |
| `Expanded` | All child nodes, controls or content of the UI automation element are displayed. |
| `LeafNode` | The UI automation element has no child nodes, controls, or content to display. |
| `PartiallyExpanded` | Some, but not all, child nodes, controls, or content of the UI automation element are displayed. |

## System.Windows.Automation.GridItemPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IGridItemProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColumnProperty` | Identifies the `Column` automation property. <br>Returns: The automation property identifier. |
| `ColumnSpanProperty` | Identifies the `ColumnSpan` automation property. <br>Returns: The automation property identifier. |
| `ContainingGridProperty` | Identifies the `ContainingGrid` automation property. <br>Returns: The automation property identifier. |
| `RowProperty` | Identifies the `Row` automation property. <br>Returns: The automation property identifier. |
| `RowSpanProperty` | Identifies the `RowSpan` property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.GridPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IGridProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColumnCountProperty` | Identifies the `ColumnCount` automation property. <br>Returns: The automation property identifier. |
| `RowCountProperty` | Identifies the `RowCount` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.MultipleViewPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IMultipleViewProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CurrentViewProperty` | Identifies the `CurrentView` automation property. <br>Returns: The automation property identifier. |
| `SupportedViewsProperty` | Identifies the automation property that gets the control-specific collection of views. <br>Returns: The automation property identifier. |

## System.Windows.Automation.Peers.AutomationControlType

Specifies the control type that is exposed to the UI automation client.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Button` | A button control. |
| `Calendar` | A calendar control, such as a date picker. |
| `CheckBox` | A check box control. |
| `ComboBox` | A combo box control. |
| `Custom` | A control that is not one of the defined control types. |
| `DataGrid` | A data grid control. |
| `DataItem` | A data item control. |
| `Document` | A document control. |
| `Edit` | An edit control, such as a text box. |
| `Group` | A group control, which acts as a container for other controls. |
| `Header` | A header control, which is a container for the labels of rows and columns of information. |
| `HeaderItem` | A header item, which is the label for a row or column of information. |
| `Hyperlink` | A hyperlink control. |
| `Image` | An image control. |
| `List` | A list control, such as a list box. |
| `ListItem` | A list item control, which is a child item of a list control. |
| `Menu` | A menu control, such as a top-level menu in an application window. |
| `MenuBar` | A menu bar control, which generally contains a set of top-level menus. |
| `MenuItem` | A menu item control. |
| `Pane` | A pane control. |
| `ProgressBar` | A progress bar control, which visually indicates the progress of a lengthy operation. |
| `RadioButton` | A radio button control, which is a selection mechanism allowing exactly one selected item in a group. |
| `ScrollBar` | A scroll bar control, such as a scroll bar in an application window. |
| `Separator` | A separator, which creates a visual division in controls such as menus and toolbars. |
| `Slider` | A slider control. |
| `Spinner` | A spinner control. |
| `SplitButton` | A split button, which is a button that performs a default action and can also expand to a list of other possible actions. |
| `StatusBar` | A status bar control. |
| `Tab` | A tab control. |
| `TabItem` | A tab item control, which represents a page of a tab control. |
| `Table` | A table. |
| `Text` | An edit control, such as a text box or rich text box. |
| `Thumb` | The control in a scrollbar that can be dragged to a different position. |
| `TitleBar` | The caption bar on a window. |
| `ToolBar` | A toolbar, such as the control that contains a set of command buttons in an application window. |
| `ToolTip` | A tooltip control, an informational window that appears as a result of moving the pointer over a control or sometimes when tabbing to a control using the keyboard. |
| `Tree` | A tree control. |
| `TreeItem` | A node in a tree control. |
| `Window` | A window frame, which contains child objects. |

## System.Windows.Automation.Peers.AutomationEvents

Specifies the event that is raised by the element through the associated `AutomationPeer`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AsyncContentLoaded` | The event that is raised when content is loaded asynchronously. |
| `AutomationFocusChanged` | The event that is raised when the focus has changed. See `SetFocus`. |
| `InvokePatternOnInvoked` | The event that is raised when a control is activated. See `Invoke`. |
| `MenuClosed` | The event that is raised when a menu is closed. |
| `MenuOpened` | The event that is raised when a menu is opened. . |
| `PropertyChanged` | The event that is raised when a property has changed. |
| `SelectionItemPatternOnElementAddedToSelection` | The event that is raised when an item is added to a collection of selected items. See `AddToSelection`. |
| `SelectionItemPatternOnElementRemovedFromSelection` | The event that is raised when an item is removed from a collection of selected items. See `RemoveFromSelection`. |
| `SelectionItemPatternOnElementSelected` | The event that is raised when a single item is selected (which clears any previous selection). See `Select`. |
| `SelectionPatternOnInvalidated` | The event that is raised when a selection in a container has changed significantly. |
| `StructureChanged` | The event that is raised when the UI Automation tree structure is changed. |
| `TextPatternOnTextChanged` | The event that is raised when textual content is modified. |
| `TextPatternOnTextSelectionChanged` | The event that is raised when the text selection is modified. |
| `ToolTipClosed` | The event that is raised when a tooltip is closed. |
| `ToolTipOpened` | The event that is raised when a tooltip is opened. |

## System.Windows.Automation.Peers.AutomationOrientation

Specifies the orientation direction in which a control can be presented.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Horizontal` | The control is presented horizontally. |
| `None` | The control does not have an orientation. |
| `Vertical` | The control is presented horizontally. |

## System.Windows.Automation.Peers.AutomationPeer

Provides a base class that exposes an automation peer for an associated class to UI Automation.

### Properties / fields

| Member | Description |
|---|---|
| `EventsSource` | Gets or sets an `AutomationPeer` that is reported to the automation client as a source for all the events that come from this `AutomationPeer`. <br>Returns: The `AutomationPeer` that is the source of events. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Provides initialization for base class values when they are called by the constructor of a derived class. |
| `GetAcceleratorKey` | Gets the accelerator key combination for the object that is associated with the UI Automation peer. <br>Returns: The accelerator key combination hint string. |
| `GetAcceleratorKeyCore` | When overridden in a derived class, is called by `GetAcceleratorKey`. <br>Returns: The accelerator key. |
| `GetAccessKey` | Gets the access key for the object that is associated with the automation peer. <br>Returns: The access key. |
| `GetAccessKeyCore` | When overridden in a derived class, is called by `GetAccessKey`. <br>Returns: The access key. |
| `GetAutomationControlType` | Gets the control type for the object that is associated with the UI Automation peer. <br>Returns: The control type, as a value of the enumeration. |
| `GetAutomationControlTypeCore` | When overridden in a derived class, is called by `GetAutomationControlType`. <br>Returns: The control type, as a value of the enumeration. |
| `GetAutomationId` | Gets the `AutomationId` of the object that is associated with the automation peer. <br>Returns: The automation identifier. |
| `GetAutomationIdCore` | When overridden in a derived class, is called by `GetAutomationId`. <br>Returns: The automation identifier. |
| `GetBoundingRectangle` | Gets the `Rect` object that represents the screen coordinates of the element that is associated with the automation peer. <br>Returns: The bounding rectangle. |
| `GetBoundingRectangleCore` | When overridden in a derived class, is called by `GetBoundingRectangle`. <br>Returns: The bounding rectangle. |
| `GetChildren` | Gets the collection of child elements that are represented in the UI Automation tree as immediate child elements of the automation peer. <br>Returns: The collection of child elements. |
| `GetChildrenCore` | When overridden in a derived class, is called by `GetChildren`. <br>Returns: The collection of child elements. |
| `GetClassName` | Gets the name of the control class that is associated with the peer. <br>Returns: The class name of the associated control class. |
| `GetClassNameCore` | When overridden in a derived class, is called by `GetClassName`. <br>Returns: The class name of the related control class. |
| `GetClickablePoint` | Gets a `Point` on the object that is associated with the automation peer that responds to a mouse click. <br>Returns: A point in the clickable area of the element. |
| `GetClickablePointCore` | When overridden in a derived class, is called by `GetClickablePoint`. <br>Returns: A point within the clickable area of the element. |
| `GetHelpText` | Gets text that describes the functionality of the control that is associated with the automation peer. <br>Returns: The help text. |
| `GetHelpTextCore` | When overridden in a derived class, is called by `GetHelpText`. <br>Returns: The help text. |
| `GetItemStatus` | Gets text that conveys the visual status of the object that is associated with this automation peer. <br>Returns: The item status. |
| `GetItemStatusCore` | When overridden in a derived class, is called by `GetItemStatus`. <br>Returns: The item status. |
| `GetItemType` | Gets a string that describes what kind of item an element represents. <br>Returns: The kind of item. |
| `GetItemTypeCore` | When overridden in a derived class, is called by `GetItemType`. <br>Returns: The kind of item. |
| `GetLabeledBy` | Gets the `AutomationPeer` for the `UIElement` that is targeted to the element. <br>Returns: The `AutomationPeer` for the element that is targeted by the `UIElement`. |
| `GetLabeledByCore` | When overridden in a derived class, is called by `GetLabeledBy`. <br>Returns: The `AutomationPeer` for the element that is targeted by the `UIElement`. |
| `GetLocalizedControlType` | Gets a localized string that represents the control type, for the control that is associated with this automation peer. The localized string parallels a `AutomationControlType` value. <br>Returns: A string that reports the localized type of the associated control. |
| `GetLocalizedControlTypeCore` | When overridden in a derived class, is called by `GetLocalizedControlType`. <br>Returns: A string that reports the localized type of the associated control. |
| `GetName` | Gets the value that the automation peer reports as the UI Automation Name for the associated control. <br>Returns: The value to report as the UI Automation Name. |
| `GetNameCore` | When overridden in a derived class, is called by `GetName`. <br>Returns: The name. |
| `GetOrientation` | Gets a value that indicates the explicit control orientation, if any. <br>Returns: The orientation of the control as a value of the enumeration. |
| `GetOrientationCore` | When overridden in a derived class, is called by `GetOrientation`. <br>Returns: The orientation of the control. |
| `GetParent` | Gets the `AutomationPeer` that is the parent of this `AutomationPeer`. <br>Returns: The parent automation peer. |
| `GetPattern(Automation.Peers.PatternInterface)` | When overridden in a derived class, gets an object that supports the requested pattern, based on `PatternInterface` input and the peer's implementation of known patterns. <br>`patternInterface`: A value from the `PatternInterface` enumeration. <br>Returns: The object that implements the pattern interface; null if this peer does not support this interface. |
| `HasKeyboardFocus` | Gets a value that indicates whether the object that is associated with this automation peer currently has keyboard focus. <br>Returns: true if the element has keyboard focus; otherwise, false. |
| `HasKeyboardFocusCore` | When overridden in a derived class, is called by `HasKeyboardFocus`. <br>Returns: true if the element has keyboard focus; otherwise, false. |
| `InvalidatePeer` | Triggers recalculation of the main properties of the `AutomationPeer` and raises the `PropertyChanged` notification to the automation client if the properties have changed. |
| `IsContentElement` | Gets a value that indicates whether the object that is associated with this automation peer contains data that is presented to the user. <br>Returns: true if the element is a content element; otherwise, false. |
| `IsContentElementCore` | When overridden in a derived class, is called by `IsContentElement`. <br>Returns: true if the element is a content element; otherwise, false. |
| `IsControlElement` | Gets a value that indicates whether the element is understood by the user as interactive or as contributing to the logical structure of the control in the GUI. <br>Returns: true if the element is a control; otherwise, false. |
| `IsControlElementCore` | When overridden in a derived class, is called by `IsControlElement`. <br>Returns: true if the element is a control; otherwise, false. |
| `IsEnabled` | Gets a value that indicates whether the element associated with this automation peer supports interaction. <br>Returns: true if the element supports interaction; otherwise, false. |
| `IsEnabledCore` | When overridden in a derived class, is called by `IsEnabled`. <br>Returns: true if the automation peer can receive and send events; otherwise, false. |
| `IsKeyboardFocusable` | Gets a value that indicates whether the element can accept keyboard focus. <br>Returns: true if the element can accept keyboard focus; otherwise, false. |
| `IsKeyboardFocusableCore` | When overridden in a derived class, is called by `IsKeyboardFocusable`. <br>Returns: true if the element can accept keyboard focus; otherwise, false. |
| `IsOffscreen` | Gets a value that indicates whether an element is off the screen. <br>Returns: true if the element is not on the screen; otherwise, false. |
| `IsOffscreenCore` | When overridden in a derived class, is called by `IsOffscreen`. <br>Returns: true if the element is not on the screen; otherwise, false. |
| `IsPassword` | Gets a value that indicates whether the element contains sensitive content. <br>Returns: true if the element contains sensitive content such as a password; otherwise, false. |
| `IsPasswordCore` | When overridden in a derived class, is called by `IsPassword`. <br>Returns: true if the element contains sensitive content; otherwise, false. |
| `IsRequiredForForm` | Gets a value that indicates whether the object that is associated with this peer must be completed on a form. <br>Returns: true if the element must be completed; otherwise, false. |
| `IsRequiredForFormCore` | When overridden in a derived class, is called by `IsRequiredForForm`. <br>Returns: true if the element must be completed; otherwise, false. |
| `ListenerExists(Automation.Peers.AutomationEvents)` | Gets a value that indicates whether UI Automation is listening for the specified event. <br>`eventId`: One of the enumeration values. <br>Returns: true if UI Automation is listening for the specified event; otherwise, false. |
| `PeerFromProvider(Automation.Provider.IRawElementProviderSimple)` | Gets an `AutomationPeer` for the specified `IRawElementProviderSimple` proxy. <br>`provider`: The class that implements `IRawElementProviderSimple`. <br>Returns: The `AutomationPeer` for the specified `IRawElementProviderSimple` proxy. |
| `ProviderFromPeer(Automation.Peers.AutomationPeer)` | Gets the `IRawElementProviderSimple` proxy for the specified `AutomationPeer`. <br>`peer`: The automation peer. <br>Returns: The proxy. |
| `RaiseAutomationEvent(Automation.Peers.AutomationEvents)` | Raises an automation event. <br>`eventId`: The event identifier for the event to raise, as a value of the enumeration. See `AutomationEvents`. |
| `RaisePropertyChangedEvent(Automation.AutomationProperty,Object,Object)` | Raises an event to notify the automation client of a changed property value. <br>`property`: The property that changed. <br>`oldValue`: The previous value of the property. <br>`newValue`: The new value of the property. |
| `SetFocus` | Sets the keyboard focus on the object that is associated with this automation peer. |
| `SetFocusCore` | When overridden in a derived class, is called by `SetFocus`. |

## System.Windows.Automation.Peers.ButtonAutomationPeer

Exposes `Button` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Button)` | Initializes a new instance of the `ButtonAutomationPeer` class. <br>`owner`: The element associated with this automation peer. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `ButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this UI automation peer. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the specified pattern, based on the patterns supported by this `ButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IInvokeProvider#Invoke` | For a description of this member, see `Invoke`. |

## System.Windows.Automation.Peers.ButtonBaseAutomationPeer

Represents a base class for exposing classes derived from `ButtonBase` to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.ButtonBase)` | Provides initialization for base class values when called by the constructor of a derived class. <br>`owner`: The `ButtonBase` to associate with this peer. |
| `GetNameCore` | Gets the UI Automation Name for the element associated with this `ButtonBaseAutomationPeer`. Called by `GetName`. <br>Returns: The UI Automation Name for the associated element. |

## System.Windows.Automation.Peers.CheckBoxAutomationPeer

Exposes `CheckBox` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(CheckBox)` | Initializes a new instance of the `CheckBoxAutomationPeer` class. <br>`owner`: The `CheckBox` to associate with this `CheckBoxAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the `AutomationControlType` for the class associated with this `CheckBoxAutomationPeer`. Called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `CheckBoxAutomationPeer`. Called by `GetClassName`. <br>Returns: The class name. |

## System.Windows.Automation.Peers.ComboBoxAutomationPeer

Exposes `ComboBox` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IExpandCollapseProvider#ExpandCollapseState` | For a description of this member, see `ExpandCollapseState`. <br>Returns: The state of the control. |
| `System#Windows#Automation#Provider#IValueProvider#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the control is read-only; otherwise, false. |
| `System#Windows#Automation#Provider#IValueProvider#Value` | For a description of this member, see `Value`. <br>Returns: The value of the control. |

### Methods

| Member | Description |
|---|---|
| `Constructor(ComboBox)` | Initializes a new instance of the `ComboBoxAutomationPeer` class. <br>`owner`: The `ComboBox` instance to associate with this `ComboBoxAutomationPeer`. |
| `CreateItemAutomationPeer(Object)` | Creates an automation peer for an individual item in the `ComboBox` (or other element) associated with this `ComboBoxAutomationPeer`. <br>`item`: The item to create the automation peer for. <br>Returns: The new `ListBoxItemAutomationPeer` for the specified item in the children collection. |
| `GetAutomationControlTypeCore` | Gets the control type for the element associated with this `ComboBoxAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: The control type as a value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ComboBoxAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `ComboBoxAutomationPeer`. <br>`pattern`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IExpandCollapseProvider#Collapse` | For a description of this member, see `Collapse`. |
| `System#Windows#Automation#Provider#IExpandCollapseProvider#Expand` | For a description of this member, see `Expand`. |
| `System#Windows#Automation#Provider#IValueProvider#SetValue(String)` | For a description of this member, see `String)`. <br>`value`: The value of the control. |

## System.Windows.Automation.Peers.FrameworkElementAutomationPeer

Exposes `FrameworkElement` types (including controls) to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `Owner` | Gets the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. <br>Returns: The element that owns this peer class. |

### Methods

| Member | Description |
|---|---|
| `Constructor(FrameworkElement)` | Initializes a new instance of the `FrameworkElementAutomationPeer` class. <br>`owner`: The `FrameworkElement` that is associated with this `FrameworkElementAutomationPeer`. |
| `CreatePeerForElement(UIElement)` | Creates a `FrameworkElementAutomationPeer` for the specified `UIElement`. <br>`element`: The `UIElement` that is associated with this `FrameworkElementAutomationPeer`. <br>Returns: The `FrameworkElementAutomationPeer` for the specified `UIElement`. |
| `FromElement(UIElement)` | Returns the `FrameworkElementAutomationPeer` for the specified `UIElement`. <br>`element`: The `UIElement` that is associated with this `FrameworkElementAutomationPeer`. <br>Returns: The `FrameworkElementAutomationPeer`, or null if the `FrameworkElementAutomationPeer` was not created by the `UIElement)` method. |
| `GetAcceleratorKeyCore` | Returns the accelerator key for the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetAcceleratorKey`. <br>Returns: The `AcceleratorKey` that is returned by `DependencyObject)`. |
| `GetAccessKeyCore` | Returns the access key for the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetAccessKey`. <br>Returns: The access key for the element that is associated with this `FrameworkElementAutomationPeer`. |
| `GetAutomationControlTypeCore` | Returns the control type for the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetAutomationIdCore` | Returns the string that uniquely identifies the `FrameworkElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetAutomationId`. <br>Returns: The automation identifier for the element associated with the `FrameworkElementAutomationPeer`, or `Empty` if there is no automation identifier. |
| `GetBoundingRectangleCore` | Returns the `Rect` that represents the bounding rectangle of the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetBoundingRectangle`. <br>Returns: The `Rect` that contains the coordinates of the element. |
| `GetChildrenCore` | Returns the collection of child elements of the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetChildren`. <br>Returns: A list of child `AutomationPeer` elements. |
| `GetClassNameCore` | Returns the name of the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The name of the owner type that is associated with this `FrameworkElementAutomationPeer`. See "Notes for Inheritors". |
| `GetClickablePointCore` | Returns a `Point` that represents the clickable space that is on the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetClickablePoint`. <br>Returns: The `Point` on the element that allows a click. |
| `GetHelpTextCore` | Returns the string that describes the functionality of the `FrameworkElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetHelpText`. <br>Returns: The help text, or `Empty` if there is no help text. |
| `GetItemStatusCore` | Returns a string that communicates the visual status of the `UIElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetItemStatus`. <br>Returns: The string that contains the `ItemStatus` that is returned by `DependencyObject)`. |
| `GetItemTypeCore` | Returns a human-readable string that contains the item type that the `UIElement` for this `FrameworkElementAutomationPeer` represents. This method is called by `GetItemType`. <br>Returns: The string that contains the `ItemType` that is returned by `DependencyObject)`. |
| `GetLabeledByCore` | Returns the `AutomationPeer` for the `UIElement` that targets the `FrameworkElement` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetLabeledBy`. <br>Returns: The `AutomationPeer` for the element that is targeted by the `UIElement`. |
| `GetLocalizedControlTypeCore` | Returns a localized human-readable string that identifies a control type. The control type is for the owner type that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetLocalizedControlType`. <br>Returns: A localized string that contains the type name of the owner control. |
| `GetNameCore` | Returns the UI Automation Name from the element that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetName`. <br>Returns: The UI Automation Name from the element that is associated with this automation peer. |
| `GetOrientationCore` | Returns a value that indicates whether the element that is associated with this `FrameworkElementAutomationPeer` is laid out in a specific direction. This method is called by `GetOrientation`. <br>Returns: The `None` enumeration value. |
| `GetPattern(Automation.Peers.PatternInterface)` | Returns an object that supports the requested pattern, based on the patterns supported by this `FrameworkElementAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: See Remarks. |
| `HasKeyboardFocusCore` | Returns a value that indicates whether the `UIElement` that is associated with this `FrameworkElementAutomationPeer` currently has keyboard input focus. This method is called by `HasKeyboardFocus`. <br>Returns: true if the element has keyboard input focus; otherwise, false. |
| `IsContentElementCore` | Returns a value that indicates whether the element that is associated with this `FrameworkElementAutomationPeer` is an element that contains data that is presented to the user. This method is called by `IsContentElement`. <br>Returns: true if the element contains data for the user to read; otherwise, false. |
| `IsControlElementCore` | Returns a value that indicates whether the object that is associated with this `FrameworkElementAutomationPeer` is understood by the end user as interactive. Optionally, the user might understand the object as contributing to the logical structure of the control in the GUI. This method is called by `IsControlElement`. <br>Returns: true if the object is interactive; otherwise, false. |
| `IsEnabledCore` | Returns a value that indicates whether the `UIElement` that is associated with this `FrameworkElementAutomationPeer` is enabled. This method is called by `IsEnabled`. <br>Returns: true if the element is enabled; otherwise, false. |
| `IsKeyboardFocusableCore` | Returns a value that indicates whether the element that is associated with this `FrameworkElementAutomationPeer` can accept keyboard focus. This method is called by `IsKeyboardFocusable`. <br>Returns: true if the element is focusable by the keyboard; otherwise, false. |
| `IsOffscreenCore` | Returns a value that indicates whether the `UIElement` that is associated with this `FrameworkElementAutomationPeer` is off the screen. This method is called by `IsOffscreen`. <br>Returns: true if the element is not on the screen; otherwise, false. |
| `IsPasswordCore` | Returns a value that indicates whether the element that is associated with this `FrameworkElementAutomationPeer` contains protected content. This method is called by `IsPassword`. <br>Returns: true if the element contains sensitive content; otherwise, false. |
| `IsRequiredForFormCore` | Returns a value that indicates whether the element that is associated with this `FrameworkElementAutomationPeer` is required to be completed on a form. This method is called by `IsRequiredForForm`. <br>Returns: The value that is returned by `DependencyObject)`, if the value is set; otherwise, false. |
| `SetFocusCore` | Sets the keyboard input focus on the element that is associated with this `FrameworkElementAutomationPeer`. This method is called by `SetFocus`. |

## System.Windows.Automation.Peers.HyperlinkButtonAutomationPeer

Exposes `HyperlinkButton` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(HyperlinkButton)` | Initializes a new instance of the `HyperlinkButtonAutomationPeer` class. <br>`owner`: The `HyperlinkButton` to associate with the `HyperlinkButtonAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `HyperlinkButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `HyperlinkButtonAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `HyperlinkButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `IsControlElementCore` | Gets a value that indicates whether the element associated with this automation peer is an element that contains data that is presented to the user. <br>Returns: true if the element is interactive; otherwise, false. |
| `System#Windows#Automation#Provider#IInvokeProvider#Invoke` | For a description of this member, see `Invoke`. |

## System.Windows.Automation.Peers.ImageAutomationPeer

Exposes `Image` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Image)` | Initializes a new instance of the `ImageAutomationPeer` class. <br>`owner`: The `Image` to associate with this `ImageAutomationPeer`. |

## System.Windows.Automation.Peers.ItemAutomationPeer

Exposes a data item in an `Items` collection to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `Item` | Gets the requested data item in the `Items` collection that is associated with this `ItemAutomationPeer`. <br>Returns: The requested data item. |
| `ItemsControlAutomationPeer` | Gets the `ItemsControlAutomationPeer` that is associated with the `ItemsControl` for this peer's `Items`. <br>Returns: The `ItemsControlAutomationPeer` associated with the `ItemsControl` that holds the `Items` collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object,Automation.Peers.ItemsControlAutomationPeer)` | Provides initialization for base class values when called by the constructor of a derived class. <br>`item`: The data item in the `Items` collection that is associated with this `ItemAutomationPeer`. <br>`itemsControlAutomationPeer`: The `ItemsControlAutomationPeer` that is associated with the `ItemsControl` that holds the `Items` collection. |
| `Constructor(UIElement)` | Provides initialization for base class values when called by the constructor of a derived class. <br>`item`: The data item in the `Items` collection that is associated with this `ItemAutomationPeer`. |
| `GetAcceleratorKeyCore` | Returns the accelerator key for the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetAcceleratorKey`. <br>Returns: The `AcceleratorKey` that is returned by `DependencyObject)`. |
| `GetAccessKeyCore` | Returns the access key for the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetAccessKey`. <br>Returns: The access key for the element that is associated with this `ItemAutomationPeer`. |
| `GetAutomationControlTypeCore` | Returns the control type for the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetAutomationIdCore` | Returns the string that uniquely identifies the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetAutomationId`. <br>Returns: The automation identifier for the element associated with the `ItemAutomationPeer`, or `Empty` if there is no automation identifier. |
| `GetBoundingRectangleCore` | Returns the `Rect` that represents the bounding rectangle of the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetBoundingRectangle`. <br>Returns: The `Rect` that contains the coordinates of the element item. |
| `GetChildrenCore` | Returns automation peers for the collection of child elements of the owner. The owner class is associated with the `ItemAutomationPeer`. This method is called by `GetChildren`. <br>Returns: A list of `AutomationPeer` elements. |
| `GetClassNameCore` | Returns name of the class that is associated with this `ItemAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. See Remarks. |
| `GetClickablePointCore` | Returns a `Point` that represents the clickable space for the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetClickablePoint`. <br>Returns: The `Point` on the element that allows a click. |
| `GetHelpTextCore` | Returns the string that describes the functionality of the item element that is associated with this `ItemAutomationPeer`. his method is called by `GetHelpText`. <br>Returns: The help text, or `Empty` if there is no help text. |
| `GetItemStatusCore` | Returns a string that communicates the visual status of the item element that is associated with this `ItemAutomationPeer`. This method is called by `GetItemStatus`. <br>Returns: The string that contains the `ItemStatus` that is returned by `DependencyObject)`. |
| `GetItemTypeCore` | Returns a human-readable string that contains the type of item element that the item represents. This method is called by `GetItemType`. <br>Returns: The item type string. |
| `GetLabeledByCore` | Returns the `AutomationPeer` for the object that targets the item that is associated with this `ItemAutomationPeer`. This method is called by `GetLabeledBy`. <br>Returns: An automation peer for the object that label-targets the peer's owner. |
| `GetLocalizedControlTypeCore` | Returns a localized human-readable string that represents a control type. The control is the owner type that is associated with this `ItemAutomationPeer`. This method is called by `GetLocalizedControlType`. <br>Returns: The string that contains the type of control. |
| `GetNameCore` | Gets the UI Automation Name from the element that corresponds to a data item. The item is the element in an `Items` collection that is associated with this `ItemAutomationPeer`. This method is called by `GetName`. <br>Returns: The UI Automation Name from the element that corresponds to a data item. |
| `GetOrientationCore` | Returns a value that indicates whether the element that is associated with this `ItemAutomationPeer` is laid out in a specific direction. This method is called by `GetOrientation`. <br>Returns: A value of the enumeration. |
| `GetPattern(Automation.Peers.PatternInterface)` | Returns an object that supports the requested pattern, based on the patterns supported by this `ItemAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented. |
| `HasKeyboardFocusCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` currently has keyboard input focus. This method is called by `HasKeyboardFocus`. <br>Returns: true if the element has keyboard input focus; otherwise, false. |
| `IsContentElementCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` is an element that contains data that is presented to the user. This method is called by `IsContentElement`. <br>Returns: true if the element contains data for the user to read; otherwise, false. |
| `IsControlElementCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` is understood by the end user as interactive. Optionally, the user might understand the element as contributing to the logical structure of the control in the GUI. This method is called by `IsControlElement`. <br>Returns: true if the element is interactive; otherwise, false. |
| `IsEnabledCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` is enabled. This method is called by `IsEnabled`. <br>Returns: true if the element is enabled; otherwise, false. |
| `IsKeyboardFocusableCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` can accept keyboard focus. This method is called by `IsKeyboardFocusable`. <br>Returns: true if the item element is focusable by the keyboard; otherwise, false. |
| `IsOffscreenCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` is off the screen. This method is called by `IsOffscreen`. <br>Returns: true if the item element is not on the screen; otherwise, false. |
| `IsPasswordCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` contains protected content. This method is called by `IsPassword`. <br>Returns: true if the item element contains sensitive content; otherwise, false. |
| `IsRequiredForFormCore` | Returns a value that indicates whether the item element that is associated with this `ItemAutomationPeer` is required to be completed on a form. This method is called by `IsRequiredForForm`. <br>Returns: The value that is returned by `DependencyObject)`, if the value is set; otherwise, false. |
| `SetFocusCore` | Sets the keyboard input focus on the item element that is associated with this `FrameworkElementAutomationPeer`. This method is called by `SetFocus`. |

## System.Windows.Automation.Peers.ItemsControlAutomationPeer

Exposes `ItemsControl` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(ItemsControl)` | Provides initialization for base class values when called by the constructor of a derived class. <br>`owner`: The `ItemsControl` to associate with this `ItemsControlAutomationPeer`. |
| `CreateItemAutomationPeer(Object)` | Creates a new instance of the `ItemAutomationPeer` for a data item. The item exists in the `Items` collection of the associated `ItemsControl` owner. <br>`item`: The data item that is associated with this `ItemAutomationPeer`. <br>Returns: An object that exposes the data item to UI automation, or null. See Remarks. |
| `GetChildrenCore` | Gets automation peers for the collection of child elements of the element that is associated with this `ItemsControlAutomationPeer`. <br>Returns: A list of automation peers for child elements. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that implements the requested pattern, based on the patterns supported by this `ItemsControlAutomationPeer`. <br>`patternInterface`: One of the enumeration values that indicates the desired control pattern. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |

## System.Windows.Automation.Peers.ListBoxAutomationPeer

Exposes `ListBox` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(ListBox)` | Initializes a new instance of the `ListBoxAutomationPeer` class. <br>`owner`: The `ListBox` to associate with the `ListBoxAutomationPeer`. |
| `CreateItemAutomationPeer(Object)` | Creates a new instance of the `ItemAutomationPeer` for a data item. The item peer is aligned with an item in the `Items` collection of the associated `ListBox`. <br>`item`: The data item to use as basis for the created `ItemAutomationPeer`. <br>Returns: A peer object that provides UI automation support for the specified data item in the owner `ListBox`. |
| `GetAutomationControlTypeCore` | Gets the control type for the `ListBox` that is associated with this `ListBoxAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the class name of the class that is associated with this `ListBoxAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The associated class name. |

## System.Windows.Automation.Peers.ListBoxItemAutomationPeer

Exposes the items in the `Items` collection of a `ListBox` to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Object,Automation.Peers.SelectorAutomationPeer)` | Initializes a new instance of the `ListBoxItemAutomationPeer` class using the specified selector automation peer. <br>`owner`: The `ListBoxItem` instance that is associated with this `ListBoxItemAutomationPeer`. <br>`selectorAutomationPeer`: The `SelectorAutomationPeer` that is associated with the `ListBox` that holds the `Items` collection. |
| `Constructor(ListBoxItem)` | Initializes a new instance of the `ListBoxItemAutomationPeer` class. <br>`owner`: The `ListBoxItem` instance that is associated with this `ListBoxItemAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `ListBoxItemAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ListBoxItemAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `ListBoxItemAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IScrollItemProvider#ScrollIntoView` | For a description of this member, see `ScrollIntoView`. |

## System.Windows.Automation.Peers.MediaElementAutomationPeer

Exposes `MediaElement` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(MediaElement)` | Initializes a new instance of the `MediaElementAutomationPeer` class. <br>`owner`: The `MediaElement` to associate with the `MediaElementAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `MediaElementAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration |
| `GetClassNameCore` | Gets the name of the class that is associated with this `MediaElementAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |

## System.Windows.Automation.Peers.MultiScaleImageAutomationPeer

Exposes `MultiScaleImage` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(MultiScaleImage)` | Initializes a new instance of the `MultiScaleImageAutomationPeer` class. <br>`owner`: The `MultiScaleImage` to associate with the `MultiScaleImageAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the control that is associated with this `MultiScaleImageAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is is associated with this `MultiScaleImageAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |

## System.Windows.Automation.Peers.PasswordBoxAutomationPeer

Exposes `PasswordBox` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IValueProvider#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the value is read-only; false if it can be modified. |
| `System#Windows#Automation#Provider#IValueProvider#Value` | For a description of this member, see `Value`. Always throws an exception. <br>Returns: Not applicable; always throws an exception. |

### Methods

| Member | Description |
|---|---|
| `Constructor(PasswordBox)` | Initializes a new instance of the `PasswordBoxAutomationPeer` class. <br>`owner`: The `PasswordBox` that is associated with this `PasswordBoxAutomationPeer`. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `PasswordBoxAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IValueProvider#SetValue(String)` | For a description of this member, see `String)`. <br>`value`: The value to set. |

## System.Windows.Automation.Peers.PatternInterface

Specifies the control pattern that the `PatternInterface)` method returns.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Dock` | The `IDockProvider` control pattern interface. |
| `ExpandCollapse` | The `IExpandCollapseProvider` control pattern interface. |
| `Grid` | The `IGridProvider` control pattern interface. |
| `GridItem` | The `IGridItemProvider` control pattern interface. |
| `Invoke` | The `IInvokeProvider` control pattern interface. |
| `MultipleView` | The `IMultipleViewProvider` control pattern interface. |
| `RangeValue` | The `IRangeValueProvider` control pattern interface. |
| `Scroll` | The `IScrollProvider` control pattern interface. |
| `ScrollItem` | The `IScrollItemProvider` control pattern interface. |
| `Selection` | The `ISelectionProvider` control pattern interface. |
| `SelectionItem` | The `ISelectionItemProvider` control pattern interface. |
| `Table` | The `ITableProvider` control pattern interface. |
| `TableItem` | The `ITableItemProvider` control pattern interface. |
| `Text` | The `ITextProvider` control pattern interface. |
| `Toggle` | The `IToggleProvider` control pattern interface. |
| `Transform` | The `ITransformProvider` control pattern interface. |
| `Value` | The `IValueProvider` control pattern interface. |
| `Window` | The `IWindowProvider` control pattern interface. |

## System.Windows.Automation.Peers.ProgressBarAutomationPeer

Exposes `ProgressBar` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IRangeValueProvider#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: Always returns true. |
| `System#Windows#Automation#Provider#IRangeValueProvider#LargeChange` | For a description of this member, see `LargeChange`. <br>Returns: The large-change value supported by the control, or `NaN` if this option is not supported. |
| `System#Windows#Automation#Provider#IRangeValueProvider#SmallChange` | For a description of this member, see `SmallChange`. <br>Returns: The small-change value supported by the control, or `NaN` if this option is not supported. |

### Methods

| Member | Description |
|---|---|
| `Constructor(ProgressBar)` | Initializes a new instance of the `ProgressBarAutomationPeer` class. <br>`owner`: The `ProgressBar` to associate with the `ProgressBarAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `ProgressBarAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ProgressBarAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `ProgressBarAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IRangeValueProvider#SetValue(Double)` | For a description of this member, see `Double)`. Always throws an exception. <br>`val`: The value to set. |

## System.Windows.Automation.Peers.RadioButtonAutomationPeer

Exposes `RadioButton` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#ISelectionItemProvider#IsSelected` | For a description of this member, see `IsSelected`. <br>Returns: true if the element is selected; otherwise, false. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#SelectionContainer` | For a description of this member, see `SelectionContainer`. <br>Returns: The provider that supports `ISelectionProvider`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(RadioButton)` | Initializes a new instance of the `RadioButtonAutomationPeer` class. <br>`owner`: The `RadioButton` to associate with the `RadioButtonAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `RadioButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `RadioButtonAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `RadioButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#AddToSelection` | For a description of this member, see `AddToSelection`. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#RemoveFromSelection` | For a description of this member, see `RemoveFromSelection`. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#Select` | For a description of this member, see `Select`. |

## System.Windows.Automation.Peers.RangeBaseAutomationPeer

Exposes `RangeBase` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IRangeValueProvider#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the value is read-only; false if it can be modified. |
| `System#Windows#Automation#Provider#IRangeValueProvider#LargeChange` | For a description of this member, see `LargeChange`. <br>Returns: The large-change value supported by the control. |
| `System#Windows#Automation#Provider#IRangeValueProvider#Maximum` | For a description of this member, see `Maximum`. <br>Returns: The maximum value supported by the control. |
| `System#Windows#Automation#Provider#IRangeValueProvider#Minimum` | For a description of this member, see `Minimum`. <br>Returns: The minimum value supported by the control. |
| `System#Windows#Automation#Provider#IRangeValueProvider#SmallChange` | For a description of this member, see `SmallChange`. <br>Returns: The small-change value supported by the control. |
| `System#Windows#Automation#Provider#IRangeValueProvider#Value` | For a description of this member, see `Value`. <br>Returns: The value of the control. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.RangeBase)` | Initializes a new instance of the `RangeBaseAutomationPeer` class. <br>`owner`: The `RangeBase` to associate with the `RangeBaseAutomationPeer`. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns implemented by this `RangeBaseAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IRangeValueProvider#SetValue(Double)` | For a description of this member, see `Double)`. <br>`val`: The value to set. |

## System.Windows.Automation.Peers.RepeatButtonAutomationPeer

Exposes `RepeatButton` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.RepeatButton)` | Initializes a new instance of the `RepeatButtonAutomationPeer` class. <br>`owner`: The `RepeatButton` to associate with the `RepeatButtonAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `RepeatButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `RepeatButtonAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `RepeatButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IInvokeProvider#Invoke` | For a description of this member, see `Invoke`. |

## System.Windows.Automation.Peers.RichTextBoxAutomationPeer

Exposes `RichTextBox` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(RichTextBox)` | Initializes a new instance of the `RichTextBoxAutomationPeer` class. <br>`owner`: The `RichTextBox` owner of the automation peer. |
| `GetAutomationControlTypeCore` | Gets the control type for the control that is associated with this `RepeatButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetChildrenCore` | Returns automation peers for the collection of child elements of the `RichTextBox` that is associated with this `FrameworkElementAutomationPeer`. This method is called by `GetChildren`. <br>Returns: A list of `AutomationPeer` objects for child elements. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `RepeatButtonAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `RepeatButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface; or null, if the specified pattern interface is not implemented by this peer. |

## System.Windows.Automation.Peers.ScrollBarAutomationPeer

Exposes `ScrollBar` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.ScrollBar)` | Initializes a new instance of the `ScrollBarAutomationPeer` class. <br>`owner`: The `ScrollBar` to associate with the `ScrollBarAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the `ScrollBar` that is associated with this `ScrollBarAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ScrollBarAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetClickablePointCore` | Gets a clickable point for this control. <br>Returns: A point that is clickable within the control, or a point with `NaN` coordinates, if no point is clickable within the control. |
| `GetOrientationCore` | Gets the orientation of the `ScrollBar` control associated with this peer. <br>Returns: The orientation of the scroll bar, as a value of the enumeration. |

## System.Windows.Automation.Peers.ScrollViewerAutomationPeer

Exposes `ScrollViewer` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IScrollProvider#HorizontalScrollPercent` | For a description of this member, see `HorizontalScrollPercent`. <br>Returns: The horizontal scroll position as a percentage of the total content area in the control. |
| `System#Windows#Automation#Provider#IScrollProvider#HorizontalViewSize` | For a description of this member, see `HorizontalViewSize`. <br>Returns: The horizontal size of the viewable region as a percentage of the total content area in the control. |
| `System#Windows#Automation#Provider#IScrollProvider#HorizontallyScrollable` | For a description of this member, see `HorizontallyScrollable`. <br>Returns: true if the control can scroll horizontally; otherwise, false. |
| `System#Windows#Automation#Provider#IScrollProvider#VerticalScrollPercent` | For a description of this member, see `VerticalScrollPercent`. <br>Returns: The vertical scroll position as a percentage of the total content area in the control. |
| `System#Windows#Automation#Provider#IScrollProvider#VerticalViewSize` | For a description of this member, see `VerticalViewSize`. <br>Returns: The vertical size of the viewable region as a percentage of the total content area in the control. |
| `System#Windows#Automation#Provider#IScrollProvider#VerticallyScrollable` | For a description of this member, see `VerticallyScrollable`. <br>Returns: true if the control can scroll vertically; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(ScrollViewer)` | Initializes a new instance of the `ScrollViewerAutomationPeer` class. <br>`owner`: The `ScrollViewer` object that is associated with this `ScrollViewerAutomationPeer`. instance. |
| `GetAutomationControlTypeCore` | Gets the control type for the `ScrollViewer` object that is associated with this `ScrollViewerAutomationPeer` instance. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this peer. This method is called by `GetClassName`. <br>Returns: The name of the associated class. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the specified pattern, based on the patterns supported by this automation peer. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `IsControlElementCore` | Gets a value that indicates whether the element associated with this peer is understood by the user as interactive or as contributing to the logical structure in UI. <br>Returns: true value to indicate that the owner control is interactive; otherwise, false. |
| `System#Windows#Automation#Provider#IScrollProvider#Scroll(Automation.ScrollAmount,Automation.ScrollAmount)` | For a description of this member, see `ScrollAmount)`. <br>`horizontalAmount`: The horizontal increment specific to the control. <br>`verticalAmount`: The vertical increment specific to the control. |
| `System#Windows#Automation#Provider#IScrollProvider#SetScrollPercent(Double,Double)` | For a description of this member, see `Double)`. <br>`horizontalPercent`: The horizontal position as a percentage of the content area's total range. <br>`verticalPercent`: The vertical position as a percentage of the content area's total range. |

## System.Windows.Automation.Peers.SelectorAutomationPeer

Exposes `Selector` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#ISelectionProvider#CanSelectMultiple` | For a description of this member, see `CanSelectMultiple`. <br>Returns: true if multiple selection is allowed; otherwise, false. |
| `System#Windows#Automation#Provider#ISelectionProvider#IsSelectionRequired` | For a description of this member, see `IsSelectionRequired`. <br>Returns: true if selection is required; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.Selector)` | Initializes a new instance of the `SelectorAutomationPeer` class. <br>`owner`: The `Selector` instance that is associated with this `SelectorAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `SelectorAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns that are supported by this `SelectorAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#ISelectionProvider#GetSelection` | For a description of this member, see `GetSelection`. <br>Returns: A list of UI automation providers. |

## System.Windows.Automation.Peers.SelectorItemAutomationPeer

Exposes the items in a `Selector` to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#ISelectionItemProvider#IsSelected` | For a description of this member, see `IsSelected`. <br>Returns: true if an item is selected; otherwise, false. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#SelectionContainer` | For a description of this member, see `SelectionContainer`. <br>Returns: The UI automation provider. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object,Automation.Peers.SelectorAutomationPeer)` | Initializes a new instance of the `SelectorItemAutomationPeer` using the specified selector automation peer. <br>`owner`: The `UIElement` instance to associate with this `SelectorItemAutomationPeer`. <br>`selectorAutomationPeer`: The `SelectorAutomationPeer` that is associated with the control that holds the `Items` collection. |
| `Constructor(UIElement)` | Initializes a new instance of the `SelectorItemAutomationPeer` class. <br>`owner`: The `UIElement` instance to associate with this `SelectorItemAutomationPeer`. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets a object that supports the requested pattern, based on the patterns supported by this `SelectorItemAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#AddToSelection` | For a description of this member, see `AddToSelection`. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#RemoveFromSelection` | For a description of this member, see `RemoveFromSelection`. |
| `System#Windows#Automation#Provider#ISelectionItemProvider#Select` | For a description of this member, see `Select`. |

## System.Windows.Automation.Peers.SliderAutomationPeer

Exposes `Slider` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Slider)` | Initializes a new instance of the `SliderAutomationPeer` class. <br>`owner`: The `Slider` to associate with the `SliderAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `SliderAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `SliderAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetClickablePointCore` | Gets a clickable point on the pwner control. <br>Returns: A `Point` structure containing a point on the owner control that is clickable. |

## System.Windows.Automation.Peers.TextBlockAutomationPeer

Exposes `TextBlock` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(TextBlock)` | Initializes a new instance of the `TextBlockAutomationPeer` class. <br>`owner`: The `TextBlock` to associate with the `TextBlockAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the `TextBlock` that is associated with this `TextBlockAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `TextBlockAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |
| `GetNameCore` | Gets the UI Automation Name for the `TextBlock` that is associated with this `TextBlockAutomationPeer`. Called by `GetName`. <br>Returns: The UI Automation Name of the object that is associated with this automation peer. |
| `IsControlElementCore` | Gets a value that indicates whether the `TextBlock` associated with this peer is understood by the user as interactive or as contributing to the logical structure of the UI. <br>Returns: false if the element is part of an applied template; otherwise, true. |

## System.Windows.Automation.Peers.TextBoxAutomationPeer

Exposes `TextBox` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IValueProvider#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the value is read-only; false if it can be modified. |
| `System#Windows#Automation#Provider#IValueProvider#Value` | For a description of this member, see `Value`. <br>Returns: The value of the control as a string. |

### Methods

| Member | Description |
|---|---|
| `Constructor(TextBox)` | Initializes a new instance of the `TextBoxAutomationPeer` class. <br>`owner`: The `TextBox` that is associated with this `TextBoxAutomationPeer`. |
| `GetNameCore` | Gets the UI Automation Name value from the element that is associated with this `TextBoxAutomationPeer`. Called by `GetName`. <br>Returns: The UI Automation Name of the element that is associated with this automation peer. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `TextBoxAutomationPeer`. <br>`patternInterface`: One of the enumeration values. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IValueProvider#SetValue(String)` | For a description of this member, see `String)`. <br>`value`: The new value. |

## System.Windows.Automation.Peers.ThumbAutomationPeer

Exposes `Thumb` types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.Thumb)` | Initializes a new instance of the `ThumbAutomationPeer` class. <br>`owner`: The `Thumb` to associate with the `ThumbAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `ThumbAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ThumbAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The class name. |

## System.Windows.Automation.Peers.ToggleButtonAutomationPeer

Exposes `ToggleButton` types to UI automation.

### Properties / fields

| Member | Description |
|---|---|
| `System#Windows#Automation#Provider#IToggleProvider#ToggleState` | For a description of this member, see `ToggleState`. <br>Returns: The toggle state of the control. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Primitives.ToggleButton)` | Initializes a new instance of the `ToggleButtonAutomationPeer` class. <br>`owner`: The `ToggleButton` to associate with this `ToggleButtonAutomationPeer`. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with this `ToggleButtonAutomationPeer`. This method is called by `GetAutomationControlType`. <br>Returns: A value of the enumeration. |
| `GetClassNameCore` | Gets the name of the class that is associated with this `ToggleButtonAutomationPeer`. This method is called by `GetClassName`. <br>Returns: The name of the associated class. |
| `GetPattern(Automation.Peers.PatternInterface)` | Gets an object that supports the requested pattern, based on the patterns supported by this `ToggleButtonAutomationPeer`. <br>`patternInterface`: One of the enumeration values that indicates the control pattern. <br>Returns: The object that implements the pattern interface, or null if the specified pattern interface is not implemented by this peer. |
| `System#Windows#Automation#Provider#IToggleProvider#Toggle` | For a description of this member, see `Toggle`. |

## System.Windows.Automation.Provider.IDockProvider

Exposes methods and properties to support access by a UI automation client to controls that expose their dock properties in a docking container.

### Properties / fields

| Member | Description |
|---|---|
| `DockPosition` | Gets the current `DockPosition` of the control in a docking container. <br>Returns: The `DockPosition` of the control, relative to the boundaries of the docking container and to other elements in the container. |

### Methods

| Member | Description |
|---|---|
| `SetDockPosition(Automation.DockPosition)` | Docks the control in a docking container. <br>`dockPosition`: The dock position, relative to the boundaries of the docking container and to other elements in the container. |

## System.Windows.Automation.Provider.IExpandCollapseProvider

Exposes methods and properties to support access by a UI automation client to controls that visually expand to display content and that collapse to hide content.

### Properties / fields

| Member | Description |
|---|---|
| `ExpandCollapseState` | Gets the state (expanded or collapsed) of the control. <br>Returns: The state (expanded or collapsed) of the control. |

### Methods

| Member | Description |
|---|---|
| `Collapse` | Hides all nodes, controls, or content that are descendants of the control. |
| `Expand` | Displays all child nodes, controls, or content of the control. |

## System.Windows.Automation.Provider.IGridItemProvider

Exposes methods and properties to support access by a UI automation client to individual child controls of containers that implement `IGridProvider`.

### Properties / fields

| Member | Description |
|---|---|
| `Column` | Gets the ordinal number of the column that contains the cell or item. <br>Returns: A zero-based ordinal number that identifies the column that contains the cell or item. |
| `ColumnSpan` | Gets the number of columns that are spanned by a cell or item. <br>Returns: The number of columns. |
| `ContainingGrid` | Gets a UI automation provider that implements `IGridProvider` and that represents the container of the cell or item. <br>Returns: A UI automation provider that implements the `Grid` control pattern and that represents the cell or item container. |
| `Row` | Gets the ordinal number of the row that contains the cell or item. <br>Returns: A zero-based ordinal number that identifies the row that contains the cell or item. |
| `RowSpan` | Gets the number of rows spanned by a cell or item. <br>Returns: The number of rows. |

## System.Windows.Automation.Provider.IGridProvider

Exposes methods and properties to support access by a UI automation client to controls that act as containers for a collection of child elements.

### Properties / fields

| Member | Description |
|---|---|
| `ColumnCount` | Gets the total number of columns in a grid. <br>Returns: The total number of columns in a grid. |
| `RowCount` | Gets the total number of rows in a grid. <br>Returns: The total number of rows in a grid. |

### Methods

| Member | Description |
|---|---|
| `GetItem(Int32,Int32)` | Retrieves the UI automation provider for the specified cell. <br>`row`: The ordinal number of the row that contains the cell. <br>`column`: The ordinal number of the column that contains the cell. <br>Returns: The UI automation provider for the specified cell. |

## System.Windows.Automation.Provider.IInvokeProvider

Exposes a method to support UI automation access to controls that initiate or perform a single, unambiguous action and do not maintain state when activated.

### Methods

| Member | Description |
|---|---|
| `Invoke` | Sends a request to activate a control and initiate its single, unambiguous action. |

## System.Windows.Automation.Provider.IMultipleViewProvider

Exposes methods and properties to support UI automation client access to controls that provide, and are able to switch between, multiple representations of the same set of information or child controls.

### Properties / fields

| Member | Description |
|---|---|
| `CurrentView` | Gets the current control-specific view. <br>Returns: The value () for the current view of the UI automation element. |

### Methods

| Member | Description |
|---|---|
| `GetSupportedViews` | Retrieves a collection of control-specific view identifiers. <br>Returns: A collection of values that identifies the views available for a UI automation element. |
| `GetViewName(Int32)` | Retrieves the name of a control-specific view. <br>`viewId`: The view identifier. <br>Returns: A localized name for the view. |
| `SetCurrentView(Int32)` | Sets the current control-specific view. <br>`viewId`: A view identifier. |

## System.Windows.Automation.Provider.IRangeValueProvider

Exposes methods and properties to support access by a UI automation client to controls that can be set to a value within a range.

### Properties / fields

| Member | Description |
|---|---|
| `IsReadOnly` | Gets a value that indicates whether the value of a control is read-only. <br>Returns: true if the value is read-only; false if it can be modified. |
| `LargeChange` | Gets the value that is added to or subtracted from the `Value` property when a large change is made, such as with the PAGE DOWN key. <br>Returns: The large-change value that is supported by the control, or null if the control does not support `LargeChange`. |
| `Maximum` | Gets the maximum range value that is supported by the control. <br>Returns: The maximum value that is supported by the control, or null if the control does not support `Maximum`. |
| `Minimum` | Gets the minimum range value that is supported by the control. <br>Returns: The minimum value that is supported by the control, or null if the control does not support `Minimum`. |
| `SmallChange` | Gets the value that is added to or subtracted from the `Value` property when a small change is made, such as with an arrow key. <br>Returns: The small-change value supported by the control, or null if the control does not support `SmallChange`. |
| `Value` | Gets the value of the control. <br>Returns: The value of the control, or null if the control does not support `Value`. |

### Methods

| Member | Description |
|---|---|
| `SetValue(Double)` | Sets the value of the control. <br>`value`: The value to set. |

## System.Windows.Automation.Provider.IRawElementProviderSimple

Provides methods and properties that expose basic information about a UI element.

## System.Windows.Automation.Provider.IScrollItemProvider

Exposes methods and properties to support access by UI automation client to individual child controls of containers that implement `IScrollProvider`.

### Methods

| Member | Description |
|---|---|
| `ScrollIntoView` | Scrolls the content area of a container object in order to display the control within the visible region (viewport) of the container. |

## System.Windows.Automation.Provider.IScrollProvider

Exposes methods and properties to support access by a UI automation client to a control that acts as a scrollable container for a collection of child objects. The children of this control must implement `IScrollItemProvider`.

### Properties / fields

| Member | Description |
|---|---|
| `HorizontalScrollPercent` | Gets the current horizontal scroll position. <br>Returns: The horizontal scroll position as a percentage of the total content area within the control. |
| `HorizontalViewSize` | Gets the current horizontal view size. <br>Returns: The horizontal size of the viewable region as a percentage of the total content area within the control. |
| `HorizontallyScrollable` | Gets a value that indicates whether the control can scroll horizontally. <br>Returns: true if the control can scroll horizontally; otherwise, false. |
| `VerticalScrollPercent` | Gets the current vertical scroll position. <br>Returns: The vertical scroll position as a percentage of the total content area within the control. |
| `VerticalViewSize` | Gets the vertical view size. <br>Returns: The vertical size of the viewable region as a percentage of the total content area within the control. |
| `VerticallyScrollable` | Gets a value that indicates whether the control can scroll vertically. <br>Returns: true if the control can scroll vertically; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Scroll(Automation.ScrollAmount,Automation.ScrollAmount)` | Scrolls the visible region of the content area horizontally, vertically, or both. <br>`horizontalAmount`: The horizontal increment that is specific to the control. Pass `NoScroll` if the control cannot be scrolled in this direction. <br>`verticalAmount`: The vertical increment that is specific to the control. Pass `NoScroll` if the control cannot be scrolled in this direction. |
| `SetScrollPercent(Double,Double)` | Sets the horizontal and vertical scroll position as a percentage of the total content area within the control. <br>`horizontalPercent`: The horizontal position as a percentage of the content area's total range. Pass `NoScroll` if the control cannot be scrolled in this direction. <br>`verticalPercent`: The vertical position as a percentage of the content area's total range. Pass `NoScroll` if the control cannot be scrolled in this direction. |

## System.Windows.Automation.Provider.ISelectionItemProvider

Exposes methods and properties to support access by a UI automation client to individual, selectable child controls of containers that implement `ISelectionProvider`.

### Properties / fields

| Member | Description |
|---|---|
| `IsSelected` | Gets a value that indicates whether an item is selected. <br>Returns: true if the element is selected; otherwise, false. |
| `SelectionContainer` | Gets the UI automation provider that implements `ISelectionProvider` and acts as the container for the calling object. <br>Returns: The UI automation provider. |

### Methods

| Member | Description |
|---|---|
| `AddToSelection` | Adds the current element to the collection of selected items. |
| `RemoveFromSelection` | Removes the current element from the collection of selected items. |
| `Select` | Clears any existing selection and then selects the current element. |

## System.Windows.Automation.Provider.ISelectionProvider

Exposes methods and properties to support access by a UI Automation client to controls that act as containers for a collection of individual, selectable child items. The children of this control must implement `ISelectionItemProvider`.

### Properties / fields

| Member | Description |
|---|---|
| `CanSelectMultiple` | Gets a value that indicates whether the UI automation provider allows more than one child element to be selected concurrently. <br>Returns: true if multiple selection is allowed; otherwise, false. |
| `IsSelectionRequired` | Gets a value that indicates whether the UI automation provider requires at least one child element to be selected. <br>Returns: true if selection is required; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `GetSelection` | Retrieves a UI automation provider for each child element that is selected. <br>Returns: A generic list of UI automation providers. |

## System.Windows.Automation.Provider.ITableItemProvider

Exposes methods and properties to support UI automation client access to child controls of containers that implement `ITableProvider`.

### Methods

| Member | Description |
|---|---|
| `GetColumnHeaderItems` | Retrieves an array of UI automation providers representing all the column headers associated with a table item or cell. <br>Returns: An array of UI automation providers. |
| `GetRowHeaderItems` | Retrieves an array of UI automation providers representing all the row headers associated with a table item or cell. <br>Returns: An array of UI automation providers. |

## System.Windows.Automation.Provider.ITableProvider

Exposes methods and properties to support access by a UI automation client to controls that act as containers for a collection of child elements. The children of this element must implement `ITableItemProvider` and be organized in a two-dimensional logical coordinate system that can be traversed (a UI automation client can move to adjacent controls, which are headers or cells of the table) by using the keyboard.

### Properties / fields

| Member | Description |
|---|---|
| `RowOrColumnMajor` | Gets the primary direction of traversal for the table. <br>Returns: The primary direction of traversal, as a value of the enumeration. |

### Methods

| Member | Description |
|---|---|
| `GetColumnHeaders` | Returns a collection of UI Automation providers that represents all the column headers in a table. <br>Returns: An array of UI automation providers. |
| `GetRowHeaders` | Returns a collection of UI Automation providers that represents all row headers in the table. <br>Returns: An array of UI automation providers. |

## System.Windows.Automation.Provider.ITextProvider

Exposes methods and properties to support UI Automation client access to controls that contain text.

### Properties / fields

| Member | Description |
|---|---|
| `DocumentRange` | Gets a text range that encloses the main text of a document. <br>Returns: A text range that encloses the main text of a document. |
| `SupportedTextSelection` | Gets a value that specifies whether a text provider supports selection, and if it does, the type of selection that is supported. <br>Returns: A value of `SupportedTextSelection`. |

### Methods

| Member | Description |
|---|---|
| `GetSelection` | Retrieves a collection of disjoint text ranges that are associated with the current text selection or selections. <br>Returns: A collection of disjoint text ranges. |
| `GetVisibleRanges` | Retrieves an array of disjoint text ranges from a text container. Each text range begins with the first partially visible line and ends with the last partially visible line. <br>Returns: The collection of visible text ranges within a container or an empty array. This method never returns null. |
| `RangeFromChild(Automation.Provider.IRawElementProviderSimple)` | Retrieves a text range that encloses a child element, such as an image, hyperlink, or other embedded object. <br>`childElement`: The enclosed object. <br>Returns: A range that spans the child element. |
| `RangeFromPoint(Point)` | Returns the degenerate (empty) text range that is nearest to the specified screen coordinates. <br>`screenLocation`: The location, in screen coordinates. <br>Returns: A degenerate range nearest the specified location. This method never returns null. |

## System.Windows.Automation.Provider.ITextRangeProvider

Exposes methods and properties to support UI automation client access to a span of continuous text in a text container that implements `ITextProvider`.

### Methods

| Member | Description |
|---|---|
| `AddToSelection` | Adds to the collection of highlighted text in a text container that supports multiple disjoint selections. |
| `Clone` | Returns a new `ITextRangeProvider` that is identical to the original `ITextRangeProvider` and that inherits all the properties of the original. <br>Returns: The new text range. This method never returns null. |
| `Compare(Automation.Provider.ITextRangeProvider)` | Returns a value that indicates whether the span (the `Start` endpoint through the `End` endpoint) of a text range is the same as another text range. <br>`range`: A text range to compare. <br>Returns: true if the span of both text ranges is identical; otherwise, false. |
| `CompareEndpoints(Automation.Text.TextPatternRangeEndpoint,Automation.Provider.ITextRangeProvider,Automation.Text.TextPatternRangeEndpoint)` | Returns a value that specifies whether two text ranges have identical endpoints. <br>`endpoint`: The `Start` or `End` endpoint of the caller. <br>`targetRange`: The target range for comparison. <br>`targetEndpoint`: The `Start` or `End` endpoint of the target. <br>Returns: Returns a negative value if the caller's endpoint occurs earlier in the text than the target endpoint. Returns zero if the caller's endpoint is at the same location as the target endpoint. Returns a positive value if the caller's endpoint occurs later in the text than the target endpoint. |
| `ExpandToEnclosingUnit(Automation.Text.TextUnit)` | Expands the text range to the specified text unit. <br>`unit`: The textual unit. |
| `FindAttribute(Int32,Object,Boolean)` | Returns a text range subset that has the specified attribute ID and attribute value. <br>`attributeId`: The attribute ID to search for. <br>`value`: The attribute value to search for. This value must match the specified attribute type. <br>`backward`: true if the last occurring text range should be returned instead of the first; otherwise, false. <br>Returns: A text range that has a matching attribute ID and attribute value; otherwise, null. |
| `FindText(String,Boolean,Boolean)` | Returns a text range subset that contains the specified text. <br>`text`: The text string to search for. <br>`backward`: true to return the last occurring text range instead of the first; otherwise, false. <br>`ignoreCase`: true to ignore case; otherwise, false. <br>Returns: A text range that matches the specified text; otherwise, null. |
| `GetAttributeValue(Int32)` | Retrieves the value of the specified attribute ID across the text range. <br>`attributeId`: The text attribute ID. <br>Returns: Retrieves an object that represents the value of the specified attribute. See Remarks. |
| `GetBoundingRectangles` | Retrieves a collection of bounding rectangles for each fully or partially visible line of text in a text range. <br>Returns: An array of bounding rectangles for each full or partial line of text in a text range. See Remarks. |
| `GetChildren` | Retrieves a collection of all the embedded objects that exist within the text range. <br>Returns: A collection of child objects that exist within the range. Child objects that overlap with the text range but are not completely enclosed by it are also included in the collection. Returns an empty collection if no child objects exist. |
| `GetEnclosingElement` | Returns the innermost element that encloses the text range. <br>Returns: The enclosing control, typically the text provider that provides the text range. However, if the text provider supports child text elements such as tables or hyperlinks, the enclosing element can be a descendant of the text provider. |
| `GetText(Int32)` | Retrieves the plain text of the range. <br>`maxLength`: The maximum length of the string to return. Use -1 to specify an unlimited length. <br>Returns: The plain text of the text range, which might represent a portion of the full string truncated at the specified . |
| `Move(Automation.Text.TextUnit,Int32)` | Moves the text range the specified number of text units. <br>`unit`: The text unit boundary. <br>`count`: The number of text units to move.A positive value moves the text range forward; a negative value moves the text range backward; and a value of 0 has no effect. <br>Returns: The number of units actually moved. This value can be less than the `count` requested if either of the new text range endpoints is greater than or less than the `DocumentRange` endpoints. |
| `MoveEndpointByRange(Automation.Text.TextPatternRangeEndpoint,Automation.Provider.ITextRangeProvider,Automation.Text.TextPatternRangeEndpoint)` | Moves one endpoint of a text range to the specified endpoint of a second text range. <br>`endpoint`: The endpoint to move. <br>`targetRange`: Another range from the same text provider. <br>`targetEndpoint`: An endpoint on the other range. |
| `MoveEndpointByUnit(Automation.Text.TextPatternRangeEndpoint,Automation.Text.TextUnit,Int32)` | Moves one endpoint of the text range the specified number of text units within the document range. <br>`endpoint`: The endpoint to move. <br>`unit`: The textual unit for moving. <br>`count`: The number of units to move. A positive value moves the endpoint forward; a negative value moves the endpoint backward; and a value of 0 has no effect. <br>Returns: The number of units actually moved. This value can be less than the requested if moving the endpoint extends beyond the start or end of the document. |
| `RemoveFromSelection` | From the collection of highlighted text in a text container that supports multiple disjoint selections, removes a highlighted section of text that corresponds to the caller's `Start` and `End` endpoints. |
| `ScrollIntoView(Boolean)` | Causes the text control to scroll vertically until the text range is visible in the viewport. <br>`alignToTop`: true if the text control should be scrolled so that the text range is flush with the top of the viewport; false if the text range is flush with the bottom of the viewport. |
| `Select` | Highlights text in the text control that corresponds to the `Start` and `End` endpoints of the text range. |

## System.Windows.Automation.Provider.IToggleProvider

Exposes methods and properties to support UI automation client access to controls that can cycle through a set of states and maintain a particular state.

### Properties / fields

| Member | Description |
|---|---|
| `ToggleState` | Gets the toggle state of the control. <br>Returns: The toggle state of the control, as a value of the enumeration. |

### Methods

| Member | Description |
|---|---|
| `Toggle` | Cycles through the toggle states of a control. |

## System.Windows.Automation.Provider.ITransformProvider

Exposes methods and properties to support access by a UI automation client to controls or elements that can be moved, resized, or rotated within a two-dimensional space.

### Properties / fields

| Member | Description |
|---|---|
| `CanMove` | Gets a value that indicates whether the element can be moved. <br>Returns: true if the element can be moved; otherwise, false. |
| `CanResize` | Gets a value that indicates whether the element can be resized. <br>Returns: true if the element can be resized; otherwise, false. |
| `CanRotate` | Gets a value that indicates whether the element can be rotated. <br>Returns: true if the element can be rotated; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Move(Double,Double)` | Moves the control. <br>`x`: The absolute screen coordinates of the left side of the control. <br>`y`: The absolute screen coordinates of the top of the control. |
| `Resize(Double,Double)` | Resizes the control. <br>`width`: The new width of the window, in pixels. <br>`height`: The new height of the window, in pixels. |
| `Rotate(Double)` | Rotates the control. <br>`degrees`: The number of degrees to rotate the control. A positive number rotates the control clockwise. A negative number rotates the control counterclockwise. |

## System.Windows.Automation.Provider.IValueProvider

Exposes methods and properties to support access by a UI automation client to controls that have an intrinsic value that does not span a range and that can be represented as a string.

### Properties / fields

| Member | Description |
|---|---|
| `IsReadOnly` | Gets a value that indicates whether the value of a control is read-only. <br>Returns: true if the value is read-only; false if it can be modified. |
| `Value` | Gets the value of the control. <br>Returns: The value of the control. |

### Methods

| Member | Description |
|---|---|
| `SetValue(String)` | Sets the value of a control. <br>`value`: The value to set. The provider is responsible for converting the value to the appropriate data type. |

## System.Windows.Automation.Provider.IWindowProvider

Exposes methods and properties to support access by a UI automation client to controls that provide fundamental window-based functionality within a traditional graphical user interface (GUI).

### Properties / fields

| Member | Description |
|---|---|
| `InteractionState` | Gets the interaction state of the window. <br>Returns: The interaction state of the control, as a value of the enumeration. |
| `IsModal` | Gets a value that specifies whether the window is modal. <br>Returns: true if the window is modal; otherwise, false. |
| `IsTopmost` | Gets a value that specifies whether the window is the topmost element in the z-order of layout. <br>Returns: true if the window is topmost; otherwise, false. |
| `Maximizable` | Gets a value that specifies whether the window can be maximized. <br>Returns: true if the window can be maximized; otherwise, false. |
| `Minimizable` | Gets a value that specifies whether the window can be minimized. <br>Returns: true if the window can be minimized; otherwise, false. |
| `VisualState` | Gets the visual state of the window. <br>Returns: The visual state of the window, as a value of the enumeration. |

### Methods

| Member | Description |
|---|---|
| `Close` | Closes the window. |
| `SetVisualState(Automation.WindowVisualState)` | Changes the visual state of the window (such as minimizing or maximizing it). <br>`state`: The visual state of the window to change to, as a value of the enumeration. |
| `WaitForInputIdle(Int32)` | Blocks the calling code for the specified time or until the associated process enters an idle state, whichever completes first. <br>`milliseconds`: The amount of time, in milliseconds, to wait for the associated process to become idle. <br>Returns: true if the window has entered the idle state; false if the timeout occurred. |

## System.Windows.Automation.RangeValuePatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IRangeValueProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsReadOnlyProperty` | Identifies the `IsReadOnly` automation property. <br>Returns: The automation property identifier. |
| `LargeChangeProperty` | Identifies the `LargeChange` automation property. <br>Returns: The automation property identifier. |
| `MaximumProperty` | Identifies the `Maximum` automation property. <br>Returns: The automation property identifier. |
| `MinimumProperty` | Identifies the `Minimum` automation property. <br>Returns: The automation property identifier. |
| `SmallChangeProperty` | Identifies the `SmallChange` automation property. <br>Returns: The automation property identifier. |
| `ValueProperty` | Identifies the `Value` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.RowOrColumnMajor

Specifies whether data in a table should be read primarily by row or by column.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColumnMajor` | Data in the table should be read column by column. |
| `Indeterminate` | The best way to present the data is indeterminate. |
| `RowMajor` | Data in the table should be read row by row. |

## System.Windows.Automation.ScrollAmount

Contains values that are used by the `IScrollProvider` pattern to indicate the direction and distance to scroll.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `LargeDecrement` | Specifies that scrolling is performed in large decrements, which is equivalent to pressing the PAGE UP key or to clicking a blank part of a scrollbar. If the distance represented by the PAGE UP key is not a relevant amount for the control, or if no scrollbar exists, the value represents an amount equal to the size of the currently visible window. |
| `LargeIncrement` | Specifies that scrolling is performed in large increments, which is equivalent to pressing the PAGE DOWN key or to clicking a blank part of a scrollbar. If the distance represented by the PAGE DOWN key is not a relevant amount for the control, or if no scrollbar exists, the value represents an amount equal to the size of the currently visible region. |
| `NoAmount` | Specifies that scrolling should not be performed. |
| `SmallDecrement` | Specifies that scrolling is performed in small decrements, which is equivalent to pressing an arrow key or to clicking the arrow button on a scrollbar. |
| `SmallIncrement` | Specifies that scrolling is performed in small increments, which equivalent to pressing an arrow key or to clicking the arrow button on a scrollbar. |

## System.Windows.Automation.ScrollPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IScrollProvider` pattern, as well as the `NoScroll` constant.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `HorizontalScrollPercentProperty` | Identifies the `HorizontalScrollPercent` automation property. <br>Returns: The automation property identifier. |
| `HorizontalViewSizeProperty` | Identifies the `HorizontalViewSize` automation property. <br>Returns: The automation property identifier. |
| `HorizontallyScrollableProperty` | Identifies the `HorizontallyScrollable` automation property. <br>Returns: The automation property identifier. |
| `NoScroll` | Specifies that scrolling should not be performed. |
| `VerticalScrollPercentProperty` | Identifies the `VerticalScrollPercent` automation property. <br>Returns: The automation property identifier. |
| `VerticalViewSizeProperty` | Identifies the `VerticalViewSize` automation property. <br>Returns: The automation property identifier. |
| `VerticallyScrollableProperty` | Identifies the `VerticallyScrollable` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.SelectionItemPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `ISelectionItemProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsSelectedProperty` | Identifies the `IsSelected` automation property. <br>Returns: The automation property identifier. |
| `SelectionContainerProperty` | Identifies the `SelectionContainer` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.SelectionPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `ISelectionProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CanSelectMultipleProperty` | Identifies the `CanSelectMultiple` automation property. <br>Returns: The automation property identifier. |
| `IsSelectionRequiredProperty` | Identifies the `IsSelectionRequired` automation property. <br>Returns: The automation property identifier. |
| `SelectionProperty` | Identifies the property that gets the selected items in a container. <br>Returns: The automation property identifier. |

## System.Windows.Automation.SupportedTextSelection

Contains values that specify whether a text provider supports selection and, if so, whether it supports a single, continuous selection or multiple, disjoint selections.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Multiple` | Supports multiple, disjoint text selections. |
| `None` | Does not support text selections. |
| `Single` | Supports a single, continuous text selection. |

## System.Windows.Automation.TableItemPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `ITableProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColumnHeaderItemsProperty` | Identifies the automation property that retrieves all the column headers associated with a table item or cell. <br>Returns: The automation property identifier. |
| `RowHeaderItemsProperty` | Identifies the automation property that retrieves all the row headers associated with a table item or cell. <br>Returns: The automation property identifier. |

## System.Windows.Automation.TablePatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `ITableProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColumnHeadersProperty` | Identifies the automation property that is accessed by the `GetColumnHeaders` method. <br>Returns: The automation property identifier. |
| `RowHeadersProperty` | Identifies the automation property that is accessed by the `GetRowHeaders` method. <br>Returns: The automation property identifier. |
| `RowOrColumnMajorProperty` | Identifies the `RowOrColumnMajor` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.Text.TextPatternRangeEndpoint

Identifies text range endpoints for methods of `ITextRangeProvider`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `End` | The endpoint of the range. |
| `Start` | The start point of the range. |

## System.Windows.Automation.Text.TextUnit

Represents pre-defined units of text for the purposes of navigation within a document.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Character` | Specifies that the text unit is one character in length. |
| `Document` | Specifies that the text unit is an entire document in length. |
| `Format` | Specifies that the text unit is the length of a single, common format specification, such as bold, italic, or similar. |
| `Line` | Specifies that the text unit is one line in length. |
| `Page` | Specifies that the text unit is one document-specific page in length. |
| `Paragraph` | Specifies that the text unit is one paragraph in length. |
| `Word` | Specifies that the text unit is one word in length. |

## System.Windows.Automation.TextPatternIdentifiers

Contains values that are used as identifiers for the `ITextProvider` class.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AnimationStyleAttribute` | Identifies the AnimationStyle attribute of a text range. |
| `BackgroundColorAttribute` | Identifies the BackgroundColor attribute of a text range. |
| `BulletStyleAttribute` | Identifies the `BulletStyle` attribute of a text range. |
| `CapStyleAttribute` | Identifies the CapStyle attribute of a text range. |
| `CultureAttribute` | Identifies the Culture (`CultureInfo`) attribute of a text range. Includes the sub-language level; for example, specifies French–Switzerland (fr-CH) instead of merely French (fr). |
| `FontNameAttribute` | Identifies the FontName attribute of a text range. |
| `FontSizeAttribute` | Identifies the FontSize attribute of a text range. |
| `FontWeightAttribute` | Identifies the FontWeight attribute of a text range. |
| `ForegroundColorAttribute` | Identifies the ForegroundColor attribute of a text range. |
| `HorizontalTextAlignmentAttribute` | Identifies the HorizontalTextAlignment attribute of a text range. |
| `IndentationFirstLineAttribute` | Identifies the IndentationFirstLine attribute of a text range. |
| `IndentationLeadingAttribute` | Identifies the IndentationLeadingattribute of a text range. |
| `IndentationTrailingAttribute` | Identifies the IndentationTrailingattribute of a text range. |
| `IsHiddenAttribute` | Identifies the IsHidden attribute of a text range. |
| `IsItalicAttribute` | Identifies the IsItalic attribute of a text range. |
| `IsReadOnlyAttribute` | Identifies the IsReadOnly attribute of a text range. |
| `IsSubscriptAttribute` | Identifies the IsSubscript attribute of a text range. |
| `IsSuperscriptAttribute` | Identifies the IsSuperscript attribute of a text range. |
| `MarginBottomAttribute` | Identifies the MarginBottom attribute of a text range. |
| `MarginLeadingAttribute` | Identifies the MarginLeading attribute of a text range. |
| `MarginTopAttribute` | Identifies the MarginTop attribute of a text range. |
| `MarginTrailingAttribute` | Identifies the MarginTrailing attribute of a text range. |
| `OutlineStylesAttribute` | Identifies the OutlineStyles attribute of a text range. |
| `OverlineColorAttribute` | Identifies the OverlineColor attribute of a text range. |
| `OverlineStyleAttribute` | Identifies the OverlineStyle attribute of a text range. |
| `StrikethroughColorAttribute` | Identifies the StrikethroughColor attribute of a text range. |
| `StrikethroughStyleAttribute` | Identifies the StrikethroughStyle attribute of a text range. |
| `TabsAttribute` | Identifies the Tabs attribute of a text range. |
| `TextFlowDirectionsAttribute` | Identifies the TextFlowDirections attribute of a text range. |
| `UnderlineColorAttribute` | Identifies the UnderlineColor attribute of a text range. |
| `UnderlineStyleAttribute` | Identifies the UnderlineStyle attribute of a text range. |

## System.Windows.Automation.TogglePatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IToggleProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ToggleStateProperty` | Identifies the `ToggleState` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.ToggleState

Contains values that specify the `ToggleState` of a UI automation element

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Indeterminate` | The UI automation element is in an indeterminate state. |
| `Off` | The UI automation element is not selected, checked, marked, or otherwise activated. |
| `On` | The UI automation element is selected, checked, marked, or otherwise activated. |

## System.Windows.Automation.TransformPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `ITransformProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CanMoveProperty` | Identifies the `CanMove` automation property. <br>Returns: The automation property identifier. |
| `CanResizeProperty` | Identifies the `CanResize` automation property. <br>Returns: The automation property identifier. |
| `CanRotateProperty` | Identifies the `CanRotate` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.ValuePatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IValueProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsReadOnlyProperty` | Identifies the `IsReadOnly` property. |
| `ValueProperty` | Identifies the `Value` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.WindowInteractionState

Defines values that specify the current state of the window for purposes of user or programmatic interaction.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BlockedByModalWindow` | The window is blocked by a modal window. |
| `Closing` | The window is closing. |
| `NotResponding` | The window is not responding. |
| `ReadyForUserInteraction` | The window is ready for user interaction. |
| `Running` | The window is running. This does not guarantee that the window is responding or ready for user interaction. |

## System.Windows.Automation.WindowPatternIdentifiers

Contains values used as automation property identifiers specifically for properties of the `IWindowProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CanMaximizeProperty` | Identifies the `Maximizable` automation property. <br>Returns: The automation property identifier. |
| `CanMinimizeProperty` | Identifies the `Minimizable` automation property. <br>Returns: The automation property identifier. |
| `IsModalProperty` | Identifies the `IsModal` automation property. <br>Returns: The automation property identifier. |
| `IsTopmostProperty` | Identifies the `IsTopmost` automation property. <br>Returns: The automation property identifier. |
| `WindowInteractionStateProperty` | Identifies the `InteractionState` automation property. <br>Returns: The automation property identifier. |
| `WindowVisualStateProperty` | Identifies the `VisualState` automation property. <br>Returns: The automation property identifier. |

## System.Windows.Automation.WindowVisualState

Contains values that specify the visual state of a window for the `IWindowProvider` pattern.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Maximized` | Specifies that the window is maximized. |
| `Minimized` | Specifies that the window is minimized. |
| `Normal` | Specifies that the window is normal (restored). |

## System.Windows.CheckAndDownloadUpdateCompletedEventArgs

Provides data for the `CheckAndDownloadUpdateCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `Error` | Gets an exception that represents an error that occurred when attempting to download an update. <br>Returns: An exception that represents the error, or null if no error occurred. |
| `UpdateAvailable` | Gets a value that indicates whether a new version of the application was successfully downloaded and will be used on the next application launch. <br>Returns: true if an update is available; otherwise, false. |

## System.Windows.CheckAndDownloadUpdateCompletedEventHandler

Represents the method that will handle the `CheckAndDownloadUpdateCompleted` event.

## System.Windows.Clipboard

Provides static methods that facilitate transferring data to and from the system clipboard. In Silverlight 5 Beta, this access is limited to Unicode text strings.

### Methods

| Member | Description |
|---|---|
| `ContainsText` | Queries the clipboard for the presence of data in the UnicodeText format. <br>Returns: true if the system clipboard contains Unicode text data; otherwise, false. |
| `GetText` | Retrieves Unicode text data from the system clipboard, if Unicode text data exists. <br>Returns: If Unicode text data is present on the system clipboard, returns a string that contains the Unicode text data. Otherwise, returns an empty string. |
| `SetText(String)` | Sets Unicode text data to store on the clipboard, for later access with `GetText`. <br>`text`: A string that contains the Unicode text data to store on the clipboard. |

## System.Windows.CornerRadius

Describes the characteristics of a rounded corner, such as can be applied to a `Border`.

### Properties / fields

| Member | Description |
|---|---|
| `BottomLeft` | Gets or sets the radius of rounding, in pixels, of the bottom left corner of the object where a `CornerRadius` is applied. <br>Returns: A `Double` that represents the radius of rounding, in pixels, of the bottom left corner of the object where a `CornerRadius` is applied. The default is 0. |
| `BottomRight` | Gets or sets the radius of rounding, in pixels, of the bottom right corner of the object where a `CornerRadius` is applied. <br>Returns: A `Double` that represents the radius of rounding, in pixels, of the bottom right corner of the object where a `CornerRadius` is applied. The default is 0. |
| `TopLeft` | Gets or sets the radius of rounding, in pixels, of the top left corner of the object where a `CornerRadius` is applied. <br>Returns: A `Double` that represents the radius of rounding, in pixels, of the top left corner of the object where a `CornerRadius` is applied. The default is 0. |
| `TopRight` | Gets or sets the radius of rounding, in pixels, of the top right corner of the object where a `CornerRadius` is applied. <br>Returns: A `Double` that represents the radius of rounding, in pixels, of the top right corner of the object where a `CornerRadius` is applied. The default is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double)` | Initializes a new `CornerRadius` structure, applying the same uniform radius to all its corners. <br>`uniformRadius`: A uniform radius applied to all four `CornerRadius` properties (`TopLeft`, `TopRight`, `BottomRight`, `BottomLeft`). |
| `Constructor(Double,Double,Double,Double)` | Initializes a new instance of the `CornerRadius` structure, applying specific radius values to its corners. <br>`topLeft`: Sets the initial `TopLeft`. <br>`topRight`: Sets the initial `TopRight`. <br>`bottomRight`: Sets the initial `BottomLeft`. <br>`bottomLeft`: Sets the initial `BottomRight`. |
| `Equals(Object)` | Compares this `CornerRadius` structure to another object for equality. <br>`obj`: The object to compare. <br>Returns: true if the two objects are equal; otherwise, false. |
| `Equals(CornerRadius)` | Compares this `CornerRadius` structure to another `CornerRadius` structure for equality. <br>`cornerRadius`: An instance of `CornerRadius` to compare for equality. <br>Returns: true if the two instances of `CornerRadius` are equal; otherwise, false. |
| `GetHashCode` | Returns the hash code of the structure. <br>Returns: A hash code for this `CornerRadius`. |
| `ToString` | Returns the string representation of the `CornerRadius` structure. <br>Returns: A `String` that represents the `CornerRadius` value. |
| `op_Equality(CornerRadius,CornerRadius)` | Compares the value of two `CornerRadius` structures for equality. <br>`cr1`: The first structure to compare. <br>`cr2`: The other structure to compare. <br>Returns: true if the two instances of `CornerRadius` are equal; otherwise, false. |
| `op_Inequality(CornerRadius,CornerRadius)` | Compares two `CornerRadius` structures for inequality. <br>`cr1`: The first structure to compare. <br>`cr2`: The other structure to compare. <br>Returns: true if the two instances of `CornerRadius` are not equal; otherwise, false. |

## System.Windows.CrossDomainAccess

Defines values that specify the access that cross-domain callers have to a Silverlight-based application.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `NoAccess` | Cross-domain callers have no access to the Silverlight application. |
| `ScriptableOnly` | Cross-domain callers have script access to the Silverlight application. |

## System.Windows.Data.Binding

Defines a binding that connects the properties of binding targets and data sources.

### Properties / fields

| Member | Description |
|---|---|
| `BindsDirectlyToSource` | Gets or sets a value that indicates whether the binding ignores any `ICollectionView` settings on the data source. <br>Returns: true if the binding binds directly to the data source; otherwise, false. |
| `Converter` | Gets or sets the converter object that is called by the binding engine to modify the data as it is passed between the source and target, or vice versa. <br>Returns: The `IValueConverter` object that modifies the data. |
| `ConverterCulture` | Gets or sets the culture to be used by the `Converter`. <br>Returns: The `CultureInfo` used by the `Converter`. |
| `ConverterParameter` | Gets or sets a parameter that can be used in the `Converter` logic. <br>Returns: A parameter to be passed to the `Converter`. This can be used in the conversion logic. The default is null. |
| `ElementName` | Gets or sets the name of the element to use as the binding source object. <br>Returns: The value of the `Name` property or x:Name Attribute of the element to bind to. The default is null. |
| `Mode` | Gets or sets a value that indicates the direction of the data flow in the binding. <br>Returns: One of the `BindingMode` values. The default is `OneWay`. |
| `NotifyOnValidationError` | Gets or sets a value that indicates whether the `BindingValidationError` event is raised on validation errors. <br>Returns: true if the `BindingValidationError` event is raised; otherwise, false. The default is false. |
| `Path` | Gets or sets the path to the binding source property. <br>Returns: The property path for the source of the binding. See `PropertyPath` or Property Path Syntax. |
| `RelativeSource` | Gets or sets the binding source by specifying its location relative to the position of the binding target. <br>Returns: The relative location of the binding source to use. The default is null. |
| `Source` | Gets or sets the data source for the binding. <br>Returns: The source object that contains the data for the binding. |
| `UpdateSourceTrigger` | Gets or sets a value that determines the timing of binding source updates for two-way bindings. <br>Returns: A value that determines when the binding source is updated. The default is `Default`. |
| `ValidatesOnDataErrors` | Gets or sets a value that indicates whether the binding engine will report validation errors from an `IDataErrorInfo` implementation on the bound data entity. <br>Returns: true if the binding engine will report `IDataErrorInfo` validation errors; otherwise, false. The default is false. |
| `ValidatesOnExceptions` | Gets or sets a value that indicates whether the binding engine will report exception validation errors. <br>Returns: true if the binding engine will report exception validation errors; otherwise, false. The default is false. |
| `ValidatesOnNotifyDataErrors` | Gets or sets a value that indicates whether the binding engine will report validation errors from an `INotifyDataErrorInfo` implementation on the bound data entity. <br>Returns: true if the binding engine will report `INotifyDataErrorInfo` validation errors; otherwise, false. The default is true. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsDebuggingEnabled` | Indicates whether data binding debugging is enabled. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Binding` class. |
| `Constructor(String)` | Initializes a new instance of the `Binding` class with an initial property path for the data source. <br>`path`: The initial property path for the source of the binding. |
| `Constructor(Data.Binding)` | Initializes a new instance of the `Binding` class with initial property values copied from the specified `Binding`. <br>`original`: The `Binding` to copy. |
| `System#ComponentModel#ISupportInitialize#BeginInit` | Signals the object that initialization is starting. |
| `System#ComponentModel#ISupportInitialize#EndInit` | Signals the object that initialization is complete. |

## System.Windows.Data.BindingBase

Provides an abstract base class for the `Binding` class.

### Properties / fields

| Member | Description |
|---|---|
| `FallbackValue` | Gets or sets the value to use when the binding is unable to return a value. <br>Returns: The value to use when the binding is unable to return a value. The default is null. |
| `StringFormat` | Gets or sets a string that specifies how to format the binding if it displays the bound value as a string. <br>Returns: A string that specifies how to format the binding if it displays the bound value as a string. The default is null. |
| `TargetNullValue` | Gets or sets the value that is used in the target when the value of the source is null. <br>Returns: The value that is used in the target when the value of the source is null. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BindingBase` class. |
| `CheckSealed` | Throws an exception if the binding has already been attached to a binding target. |
| `ProvideValue(IServiceProvider)` | Returns an object that should be set on the property where this binding and extension are applied. <br>`serviceProvider`: The object that can provide services for the markup extension. May be null; see the Remarks section for more information. <br>Returns: The value to set on the binding target property. |

## System.Windows.Data.BindingExpression

Contains information about a single binding within a `Binding` object.

### Properties / fields

| Member | Description |
|---|---|
| `DataItem` | Gets the binding source object that this `BindingExpression` uses. <br>Returns: The binding source object that this `BindingExpression` uses. |
| `ParentBinding` | Returns the `Binding` object of the current `BindingExpression`. <br>Returns: The `Binding` object of the current `BindingExpression`. |

### Methods

| Member | Description |
|---|---|
| `UpdateSource` | Sends the current binding target value to the binding source property in `TwoWay` bindings. |

## System.Windows.Data.BindingExpressionBase

Represents the base class for the `BindingExpression` class.

## System.Windows.Data.BindingMode

Describes how the data propagates in a binding.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `OneTime` | Updates the target property when the binding is created. |
| `OneWay` | Updates the target property when the binding is created. Changes to the source object can also propagate to the target. |
| `TwoWay` | Updates either the target or the source object when either changes. When the binding is created, the target property is updated from the source. |

## System.Windows.Data.BindingOperations

Provides the static `BindingBase)` method.

### Methods

| Member | Description |
|---|---|
| `SetBinding(DependencyObject,DependencyProperty,Data.BindingBase)` | Creates and associates a new `BindingExpressionBase` with the specified binding target property. <br>`target`: The target to set the binding to. <br>`dp`: The property on the target to bind. <br>`binding`: The binding to assign to the target property. <br>Returns: An object that contains information about the binding. |

## System.Windows.Data.CollectionViewGroup

Represents a group created by a `PagedCollectionView` object based on its `GroupDescriptions`.

### Properties / fields

| Member | Description |
|---|---|
| `IsBottomLevel` | Gets a value that indicates whether this group has any subgroups. <br>Returns: true if this group is at the bottom level and does not have any subgroups; otherwise, false. |
| `ItemCount` | Gets the number of data items in the subtree under this group. <br>Returns: The number of data items in the subtree under this group. |
| `Items` | Gets the items that are immediate children of the group. <br>Returns: A read-only collection of the immediate items in this group. This is either a collection of subgroups or a collection of data items if this group does not have any subgroups. |
| `Name` | Gets the name of this group. <br>Returns: The name of this group. |
| `ProtectedItemCount` | Gets or sets the number of data items in the subtree under this group. <br>Returns: The number of data items in the subtree under this group. |
| `ProtectedItems` | Gets the immediate items that are contained in this group. <br>Returns: The immediate items that are contained in this group. |

### Events

| Member | Description |
|---|---|
| `PropertyChanged` | Occurs when a property value changes. |
| `System#ComponentModel#INotifyPropertyChanged#PropertyChanged` | For a description of this member, see `PropertyChanged`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object)` | Initializes a new instance of the `CollectionViewGroup` class with the specified group name. <br>`name`: The name of the group. |
| `OnPropertyChanged(ComponentModel.PropertyChangedEventArgs)` | Raises the `PropertyChanged` event with the provided arguments. <br>`e`: The event data. |

## System.Windows.Data.CollectionViewSource

The XAML proxy of a collection view class.

### Properties / fields

| Member | Description |
|---|---|
| `Culture` | Gets or sets the cultural information for any operations of the view that might differ by culture, such as sorting. <br>Returns: The culture to use during view operations. |
| `GroupDescriptions` | Gets a collection of `GroupDescription` objects that describe how items in the collection are grouped in the view. <br>Returns: A collection of `GroupDescription` objects that describe how items in the collection are grouped in the view. |
| `SortDescriptions` | Gets a collection of `SortDescription` objects that describe how the items in the collection are sorted in the view. <br>Returns: A collection of `SortDescription` objects that describe how the items in the collection are sorted in the view. |
| `Source` | Gets or sets the collection object from which to create this view. <br>Returns: The collection object from which to create this view. The default is null. |
| `View` | Gets the view object that is currently associated with this instance of `CollectionViewSource`. <br>Returns: The view object that is currently associated with this instance of `CollectionViewSource`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SourceProperty` | Identifies the `Source` dependency property. |
| `ViewProperty` | Identifies the `View` dependency property. |

### Events

| Member | Description |
|---|---|
| `Filter` | Provides filtering logic. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CollectionViewSource` class. |
| `DeferRefresh` | Enters a defer cycle that you can use to merge changes to the view and delay automatic refresh. <br>Returns: An `IDisposable` object that you can use to dispose of the calling object. |
| `OnCollectionViewTypeChanged(Type,Type)` | Invoked when the collection view type changes. <br>`oldCollectionViewType`: The old collection view type. <br>`newCollectionViewType`: The new collection view type. |
| `OnSourceChanged(Object,Object)` | Invoked when the `Source` property changes. <br>`oldSource`: The old value of the `Source` property. <br>`newSource`: The new value of the `Source` property. |
| `System#ComponentModel#ISupportInitialize#BeginInit` | Signals the object that initialization is starting. |
| `System#ComponentModel#ISupportInitialize#EndInit` | Signals the object that initialization is complete. |

## System.Windows.Data.Debugging.BindingAction

Represents the actions that a binding can perform.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `UpdatingSource` | The binding is currently updating the source. |
| `UpdatingTarget` | The binding is currently updating the target. |
| `ValidationNotification` | The binding is currently receiving an `INotifyDataErrorInfo` notification. |

## System.Windows.Data.Debugging.BindingDebugState

Represents the state of the binding being debugged.

### Properties / fields

| Member | Description |
|---|---|
| `Action` | Gets the binding-related action being debugged. <br>Returns: The binding-related action being debugged. |
| `Binding` | Gets the binding being debugged. <br>Returns: The binding being debugged. |
| `BindingExpression` | Gets the binding expression being debugged. <br>Returns: The binding expression being debugged. |
| `Error` | Gets the error associated with the binding, if one is present. <br>Returns: The error associated with the binding, or null if there is no error. |
| `FinalSource` | Gets the final source value on which the binding being debugged will operate. <br>Returns: The final source value on which the binding will operate. |
| `LastCompletedStage` | Gets the stage that the binding was executing when an error occurred. <br>Returns: The stage that the binding was executing when an error occurred. |
| `Pipeline` | Gets an object that contains the values at different stages of binding resolution. <br>Returns: An object that contains the values at different stages of binding resolution. |
| `ValidationErrors` | Gets the validation errors associated with the binding. <br>Returns: The validation errors associated with the binding. |

### Methods

| Member | Description |
|---|---|
| `ToString` | Returns the string representation of the `BindingDebugState`. <br>Returns: The string representation of the `BindingDebugState`. |

## System.Windows.Data.Debugging.BindingStage

Represents the stages of data binding resolution.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PathResolution` | The binding is resolving the `Path`. |
| `SetSource` | The binding is attempting to set the data source. |
| `SetTarget` | The binding is attempting to set the binding target. |
| `SetValidationResult` | The binding is setting the validation result. |
| `SourceResolution` | The binding is resolving the `Source` property value. |
| `StringFormatting` | The binding is formatting the value based on the `StringFormat` property value. |
| `TypeConversion` | The binding is converting the value to the target type. |
| `Validating` | The binding is performing validation through the `IDataErrorInfo` interface of the binding source. |
| `ValueConversion` | The binding is converting the value using the `Converter` property value. |

## System.Windows.Data.Debugging.BindingValuePipeline

Contains the values of a `Binding` at different stages of resolution.

### Properties / fields

| Member | Description |
|---|---|
| `AfterFallbackValue` | Gets the binding value after applying the `FallbackValue` property value, if present. <br>Returns: The binding value after applying the `FallbackValue` property value. |
| `AfterStringFormat` | Gets the binding value after applying the `StringFormat` property value, if present. <br>Returns: The binding value after string formatting is applied. |
| `AfterStringFormatParse` | Gets the binding value after attempting to parse the `StringFormat` property value. <br>Returns: The binding value after attempting to parse the `StringFormat` property value. |
| `AfterTypeConversion` | Gets the final binding value that will be applied after any necessary type conversion. <br>Returns: The final binding value after type conversion. |
| `AfterValueConversion` | Gets the binding value after applying the `Converter` property value, if present. <br>Returns: The binding value after applying any value conversion. |
| `InitialValue` | Gets the initial binding value before the binding pipeline is executed. <br>Returns: The initial binding value before the binding pipeline is executed. |
| `IsUsingTargetNullValue` | Gets a value that indicates whether the binding value matches the `TargetNullValue` property setting, indicating that the next value in the pipeline will be null. <br>Returns: A value that indicates whether the binding value matches the `TargetNullValue` property setting, or null if this determination cannot be made yet. |

## System.Windows.Data.FilterEventArgs

Provides data for the `Filter` event.

### Properties / fields

| Member | Description |
|---|---|
| `Accepted` | Gets or sets a value that indicates whether the item passes the filter criteria. <br>Returns: true if the item passes the filter; otherwise, false. |
| `Item` | Gets the object that the filter should test. <br>Returns: The object that the filter should test. The default is null. |

## System.Windows.Data.FilterEventHandler

Represents the method that handles the `Filter` event.

## System.Windows.Data.IValueConverter

Exposes methods that allow modifying the data as it passes through the binding engine.

### Methods

| Member | Description |
|---|---|
| `Convert(Object,Type,Object,Globalization.CultureInfo)` | Modifies the source data before passing it to the target for display in the UI. <br>`value`: The source data being passed to the target. <br>`targetType`: The `Type` of data expected by the target dependency property. <br>`parameter`: An optional parameter to be used in the converter logic. <br>`culture`: The culture of the conversion. <br>Returns: The value to be passed to the target dependency property. |
| `ConvertBack(Object,Type,Object,Globalization.CultureInfo)` | Modifies the target data before passing it to the source object. This method is called only in `TwoWay` bindings. <br>`value`: The target data being passed to the source. <br>`targetType`: The `Type` of data expected by the source object. <br>`parameter`: An optional parameter to be used in the converter logic. <br>`culture`: The culture of the conversion. <br>Returns: The value to be passed to the source object. |

## System.Windows.Data.PropertyGroupDescription

Describes the grouping of items by using a property name as the criteria.

### Properties / fields

| Member | Description |
|---|---|
| `Converter` | Gets or sets a converter to apply to the property value or the item to produce the final value that is used to determine which group(s) an item belongs to. <br>Returns: The converter to apply. The default is null. |
| `PropertyName` | Gets or sets the name of the property that is used to determine which group(s) an item belongs to. <br>Returns: The name of the property that is used to determine which group(s) an item belongs to. The default is null. |
| `StringComparison` | Gets or sets a `StringComparison` value that specifies the comparison between the value of an item (as determined by `PropertyName` and `Converter`) and the name of a group. <br>Returns: An enumeration value that specifies the comparison between the value of an item and the name of a group. The default is `Ordinal`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PropertyGroupDescription` class. |
| `Constructor(String)` | Initializes a new instance of the `PropertyGroupDescription` class with the specified property name. <br>`propertyName`: The name of the property that specifies which group an item belongs to. |
| `Constructor(String,Data.IValueConverter)` | Initializes a new instance of the `PropertyGroupDescription` class with the specified property name and converter. <br>`propertyName`: The name of the property that specifies which group an item belongs to. If this parameter is null, the item itself is passed to the value converter. <br>`converter`: An `IValueConverter` object to apply to the property value or the item to produce the final value that is used to determine which group(s) an item belongs to. The converter may return a collection, which indicates that the items can appear in more than one group. |
| `Constructor(String,Data.IValueConverter,StringComparison)` | Initializes a new instance of the `PropertyGroupDescription` class with the specified property name, converter, and string comparison. <br>`propertyName`: The name of the property that specifies which group an item belongs to. If this parameter is null, the item itself is passed to the value converter. <br>`converter`: An `IValueConverter` object to apply to the property value or the item to produce the final value that is used to determine which group(s) an item belongs to. The converter may return a collection, which indicates that the items can appear in more than one group. <br>`stringComparison`: A `StringComparison` value that specifies the comparison between the value of an item and the name of a group. |
| `GroupNameFromItem(Object,Int32,Globalization.CultureInfo)` | Returns the group name(s) for the specified item. <br>`item`: The item to return group names for. <br>`level`: The level of grouping. <br>`culture`: The `CultureInfo` to supply to the converter <br>Returns: The group name(s) for the specified item. |
| `NamesMatch(Object,Object)` | Returns a value that indicates whether the group name and the item name match, which indicates that the item belongs to the group. <br>`groupName`: The name of the group to check. <br>`itemName`: The name of the item to check. <br>Returns: true if the names match, which indicates that the item belongs to the group; otherwise, false. |

## System.Windows.Data.RelativeSource

Implements a markup extension that describes the location of the binding source relative to the position of the binding target.

### Properties / fields

| Member | Description |
|---|---|
| `AncestorLevel` | Gets or sets the level of ancestor to look for, in `FindAncestor` mode. Use 1 to indicate the one nearest to the binding target element. <br>Returns: The ancestor level. Use 1 to indicate the one nearest to the binding target element. |
| `AncestorType` | Gets or sets the type of ancestor to look for. <br>Returns: The type of ancestor. The default value is null. |
| `Mode` | Gets or sets a value that describes the location of the binding source relative to the position of the binding target. <br>Returns: A value of the enumeration. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `RelativeSource` class by using the default relative source mode. |
| `Constructor(Data.RelativeSourceMode)` | Initializes a new instance of the `RelativeSource` class by using the specified relative source mode. <br>`mode`: The relative source mode to use for the related binding. |
| `ProvideValue(IServiceProvider)` | Returns an object that should be set as the value on the target object's property for this markup extension. For `RelativeSource`, this is another `RelativeSource`, using the appropriate source for the specified mode. <br>`serviceProvider`: An object that can provide services for the markup extension. In this implementation, this parameter can be null. <br>Returns: Another `RelativeSource`. |
| `System#ComponentModel#ISupportInitialize#BeginInit` | Signals the object that initialization is starting. |
| `System#ComponentModel#ISupportInitialize#EndInit` | Signals the object that initialization is complete. |

## System.Windows.Data.RelativeSourceMode

Defines constants that describe the location of the binding source relative to the position of the binding target.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FindAncestor` | Refers to the ancestor in the parent chain of the data-bound element. You can use this to bind to an ancestor of a specific type or its subclasses. This is the mode you use if you want to specify `AncestorType` and/or `AncestorLevel`. |
| `Self` | Refers to the element on which you are setting the binding and allows you to bind one property of that element to another property on the same element. |
| `TemplatedParent` | Refers to the element to which the template (in which the data-bound element exists) is applied. This is similar to setting a TemplateBinding Markup Extension and is only applicable if the `Binding` is within a template. |

## System.Windows.Data.UpdateSourceTrigger

Defines constants that indicate when a binding source is updated by its binding target in two-way binding.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Default` | The binding source is updated automatically when the binding target value changes. |
| `Explicit` | The binding source is updated only when you call the `UpdateSource` method. |

## System.Windows.DataFormats

Provides static, predefined format names for data objects. Use the named constants to identify the format of the data that you request from an `IDataObject` object.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FileDrop` | Specifies the Microsoft Windows file drop format. <br>Returns: The string specifying the Microsoft Windows file drop format. |

## System.Windows.DataObject

Provides a basic implementation of the `IDataObject` interface, which defines a format-independent mechanism for transferring data.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DataObject` class. |
| `Constructor(Object)` | Initializes a new instance of the `DataObject` class, with specified initial data. <br>`data`: An object that represents the data to store in this data object. |
| `GetData(String)` | Retrieves a data object in a specified format; the data format is specified by a string. <br>`format`: A string that specifies what format to retrieve the data as. Use the constant `FileDrop`. <br>Returns: A data object that has the data in the specified format, or null if the data is not available in the specified format. |
| `GetData(String,Boolean)` | Retrieves a data object in a specified format, optionally converting the data to the specified format. Always throws an exception. <br>`format`: A string that specifies the format in which to retrieve the data. <br>`autoConvert`: true to attempt to automatically convert the data to the specified format; false for no data format conversion. <br>Returns: Always throws an exception. |
| `GetData(Type)` | Retrieves a data object in a specified format; the data format is specified by a `Type` object. Always throws an exception. <br>`format`: A `Type` object that specifies the format in which to retrieve the data. <br>Returns: Always throws an exception. |
| `GetDataPresent(String)` | Checks to see whether the data is available in, or can be converted to, a specified format; the data format is specified by a string. <br>`format`: A string that specifies the format to check for. See the `DataFormats` class for a set of predefined data formats. <br>Returns: true if the data is in the specified format, and is not null. false if `format` is not equivalent to `FileDrop`, or if the data is null. |
| `GetDataPresent(String,Boolean)` | Checks whether the data is available in, or can be converted to, a specified format. A Boolean flag indicates whether to check if the data can be converted to the specified format, if it is not available in that format. Always throws an exception. <br>`format`: A string that specifies the format to check for. See the `DataFormats` class for a set of predefined data formats. <br>`autoConvert`: false to only check for the specified format; true to also check whether data stored in this data object can be converted to the specified format. <br>Returns: Always throws an exception. |
| `GetDataPresent(Type)` | Checks whether the data is available in, or can be converted to, a specified format. The data format is specified by a `Type` object. Always throws an exception. <br>`format`: A `Type` that specifies the format to check for. <br>Returns: Always throws an exception. |
| `GetFormats` | Returns a list of all formats that the data in this data object is stored in, or can be converted to. <br>Returns: An array of strings, with each string specifying the name of a format supported by this data object. |
| `GetFormats(Boolean)` | Returns a list of all formats that the data in this data object is stored in. A Boolean flag indicates whether to also include formats that the data can be automatically converted to. Always throws an exception. <br>`autoConvert`: true to retrieve all formats that data in this data object is stored in, or converted to; false to retrieve only formats that data stored in this data object is stored in (excluding formats that the data is not stored in, but can be automatically converted to). <br>Returns: Always throws an exception. |
| `SetData(Object)` | Stores the specified data in this data object and automatically converts the data format from the source object type. Always throws an exception. <br>`data`: The data to store in this data object. |
| `SetData(String,Object)` | Stores the specified data in this data object, along with one or more specified data formats. The data format is specified by a string. Always throws an exception. <br>`format`: A string that specifies what format to store the data in. See the `DataFormats` class for a set of predefined data formats. <br>`data`: The data to store in this data object. |
| `SetData(String,Object,Boolean)` | Stores the specified data in this data object, together with one or more specified data formats. This overload includes a Boolean flag to indicate whether the data may be converted to another format on retrieval. Always throws an exception. <br>`format`: A string that specifies the format in which to store the data. See the `DataFormats` class for a set of predefined data formats. <br>`data`: The data to store in this data object. <br>`autoConvert`: true to allow the data to be converted to another format on retrieval; false to prohibit the data from being converted to another format on retrieval. |
| `SetData(Type,Object)` | Stores the specified data in this data object, together with one or more specified data formats. The data format is specified by a `Type` class. Always throws an exception. <br>`format`: A `Type` that specifies the format in which to store the data. <br>`data`: The data to store in this data object. |

## System.Windows.DataTemplate

Describes the visual structure of a data object.

### Properties / fields

| Member | Description |
|---|---|
| `DataType` | Gets or sets the type for which this `DataTemplate` is intended. <br>Returns: The type of object to which this template is applied. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DataTemplate` class without initializing the `DataType` property. |
| `Constructor(Object)` | Initializes a new instance of the `DataTemplate` class, setting the `DataType` property to the specified value. <br>`dataType`: The initial value of the `DataType` property. |
| `LoadContent` | Creates the `UIElement` objects in the `DataTemplate`. <br>Returns: The root `UIElement` of the `DataTemplate`. |

## System.Windows.DataTemplateKey

Represents the resource key for the `DataTemplate` class.

### Properties / fields

| Member | Description |
|---|---|
| `DataType` | Gets or sets the type for which the template is intended. <br>Returns: The type of object to which the template is applied. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DataTemplateKey` class without initializing the `DataType` property. |
| `Constructor(Object)` | Initializes a new instance of the `DataTemplateKey` class, setting the `DataType` property to the specified value. <br>`dataType`: The initial value of the `DataType` property. |
| `Equals(Object)` | Returns a value that indicates whether the specified object is a `DataTemplateKey` that has the same `DataType` property value as the current `DataTemplateKey`. <br>`o`: The object to compare to the current object. <br>Returns: true if is equivalent to the current object; otherwise, false. |
| `GetHashCode` | Returns the hash code of the `DataType` property value. <br>Returns: The hash code of the `DataType` property value, or 0 if `DataType` is null. |

## System.Windows.DependencyObject

Represents an object that participates in the Silverlight dependency property system. `DependencyObject` is the immediate base class of several other important Silverlight classes, such as `UIElement`, `Geometry`, `FrameworkTemplate`, `Style`, and `ResourceDictionary`.

### Properties / fields

| Member | Description |
|---|---|
| `Dispatcher` | Gets the `Dispatcher` this object is associated with. <br>Returns: The `Dispatcher` this object is associated with. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DependencyObject` class. |
| `CheckAccess` | Determines whether the calling thread has access to this object. <br>Returns: true if the calling thread has access to this object; otherwise, false. |
| `ClearValue(DependencyProperty)` | Clears the local value of a dependency property. <br>`dp`: The `DependencyProperty` identifier of the property to clear the value for. |
| `GetAnimationBaseValue(DependencyProperty)` | Returns any base value established for a Silverlight dependency property, which would apply in cases where an animation is not active. <br>`dp`: The identifier for the desired dependency property. <br>Returns: The returned base value. |
| `GetValue(DependencyProperty)` | Returns the current effective value of a dependency property from a `DependencyObject`. <br>`dp`: The `DependencyProperty` identifier of the property to retrieve the value for. <br>Returns: Returns the current effective value. |
| `ReadLocalValue(DependencyProperty)` | Returns the local value of a dependency property, if a local value is set. <br>`dp`: The `DependencyProperty` identifier of the property to retrieve the local value for. <br>Returns: Returns the local value, or returns the sentinel value `UnsetValue` if no local value is set. |
| `SetValue(DependencyProperty,Object)` | Sets the local value of a dependency property on a `DependencyObject`. <br>`dp`: The identifier of the dependency property to set. <br>`value`: The new local value. |

## System.Windows.DependencyObjectCollection`1

Represents a collection of `DependencyObject` instances of a specified type.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of objects in the collection. <br>Returns: The number of objects in the collection. |
| `IsReadOnly` | Gets a value that indicates whether the collection can be modified. <br>Returns: true if the collection can be modified; otherwise, false. |
| `Item` | Gets or sets the object at the specified index. <br>`index`: The index of the object to get or set. <br>Returns: The object at the specified index. |
| `System#Collections#ICollection#Count` | Gets the number of objects in the collection. <br>Returns: The number of objects in the collection. |
| `System#Collections#ICollection#IsSynchronized` | Gets a value that indicates whether access to the collection is synchronized (thread safe). <br>Returns: true if access to the collection is synchronized (thread safe); otherwise, false. |
| `System#Collections#ICollection#SyncRoot` | Gets an object that can be used to synchronize access to the collection. <br>Returns: An object that can be used to synchronize access to the collection. |
| `System#Collections#IList#IsFixedSize` | Gets a value that indicates whether the collection allows the addition or removal of objects after creation. <br>Returns: true if the collection has a fixed size; otherwise, false. |
| `System#Collections#IList#IsReadOnly` | Gets a value that indicates whether the collection can be modified. <br>Returns: true if the collection can be modified; otherwise, false. |
| `System#Collections#IList#Item` | Gets or sets the object at the specified index. <br>`index`: The index of the object to get or set. <br>Returns: The object at the specified index. |

### Events

| Member | Description |
|---|---|
| `CollectionChanged` | Occurs when items in the collection are added, removed, or replaced. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DependencyObjectCollection`1` class. |
| `Add(`0)` | Adds the specified object to the end of the collection. <br>`item`: The object to add to the collection. |
| `Clear` | Removes all objects from the collection. |
| `Contains(`0)` | Gets a value that indicates whether the specified object is in the collection. <br>`item`: The object to check for in the collection. <br>Returns: true if the object is in the collection; otherwise, false. |
| `CopyTo(`0[],Int32)` | Copies the objects in the collection to the specified array, starting at the specified index. <br>`array`: The destination array. <br>`arrayIndex`: The index of the first object to copy. |
| `GetEnumerator` | Gets an enumerator that iterates through the collection. <br>Returns: An enumerator for the collection. |
| `IndexOf(`0)` | Gets the index of the specified object within the collection, or -1 if the object is not in the collection. <br>`item`: The object to locate in the collection. <br>Returns: The index of the first occurrence of within the collection, or -1 if is not in the collection. |
| `Insert(Int32,`0)` | Adds the specified object to the collection at the specified index. <br>`index`: The index at which to add the object. <br>`item`: The object to add. |
| `Remove(`0)` | Removes the specified object from the collection. <br>`item`: The object to remove. <br>Returns: true if the object was removed; false if the object was not found in the collection. |
| `RemoveAt(Int32)` | Removes the object at the specified index from the collection. <br>`index`: The index of the object to remove. |
| `System#Collections#ICollection#CopyTo(Array,Int32)` | Copies the objects in the collection to the specified array, starting at the specified index. <br>`array`: The destination array. <br>`index`: The index of the first object to copy. |
| `System#Collections#IEnumerable#GetEnumerator` | Gets an enumerator that iterates through the collection. <br>Returns: An enumerator for the collection. |
| `System#Collections#IList#Add(Object)` | Adds the specified object to the end of the collection. <br>`value`: The object to add to the collection. <br>Returns: The index at which the object was added. |
| `System#Collections#IList#Clear` | Removes all objects from the collection. |
| `System#Collections#IList#Contains(Object)` | Gets a value that indicates whether the specified object is in the collection. <br>`value`: The object to check for in the collection. <br>Returns: true if the object is in the collection; otherwise, false. |
| `System#Collections#IList#IndexOf(Object)` | Gets the index of the specified object within the collection, or -1 if the object is not in the collection. <br>`value`: The object to locate in the collection. <br>Returns: The index of the first occurrence of within the collection, or -1 if is not in the collection. |
| `System#Collections#IList#Insert(Int32,Object)` | Adds the specified object to the collection at the specified index. <br>`index`: The index at which to add the object. <br>`value`: The object to add. |
| `System#Collections#IList#Remove(Object)` | Removes the specified object from the collection. <br>`value`: The object to remove. |
| `System#Collections#IList#RemoveAt(Int32)` | Removes the object at the specified index from the collection. <br>`index`: The index of the object to remove. |

## System.Windows.DependencyProperty

Represents a dependency property that is registered with the Silverlight dependency property system. Dependency properties provide support for value expressions, data binding, animation, and property change notification.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `UnsetValue` | Specifies a static value that is used by the property system rather than null to indicate that the property exists, but does not have its value set by the property system. <br>Returns: The sentinel value for an unset value. |

### Methods

| Member | Description |
|---|---|
| `GetMetadata(Type)` | Retrieves the property metadata value for the dependency property as registered to the specified `Type`. <br>`forType`: The specific type from which to retrieve the dependency property metadata. <br>Returns: A property metadata object. |
| `Register(String,Type,Type,PropertyMetadata)` | Registers a dependency property with the specified property name, property type, owner type, and property metadata for the property. <br>`name`: The name of the dependency property to register. <br>`propertyType`: The type of the property. <br>`ownerType`: The owner type that is registering the dependency property. <br>`typeMetadata`: A property metadata instance. This can contain a `PropertyChangedCallback` implementation reference. <br>Returns: A dependency property identifier that should be used to set the value of a public static readonly field in your class. The identifier is then used both by your own code and any third-party user code to reference the dependency property later, for operations such as setting its value programmatically, or attaching a `Binding` in code. |
| `RegisterAttached(String,Type,Type,PropertyMetadata)` | Registers an attached dependency property with the specified property name, property type, owner type, and property metadata for the property. <br>`name`: The name of the dependency property to register. <br>`propertyType`: The type of the property. <br>`ownerType`: The owner type that is registering the dependency property. <br>`defaultMetadata`: A property metadata instance. This can contain a `PropertyChangedCallback` implementation reference. <br>Returns: A dependency property identifier that should be used to set the value of a public static readonly field in your class. That identifier is then used to reference the attached property later, for operations such as setting its value programmatically, or attaching a `Binding`. |

## System.Windows.DependencyPropertyChangedEventArgs

Provides data for a `PropertyChangedCallback` implementation.

### Properties / fields

| Member | Description |
|---|---|
| `NewValue` | Gets the value of the property after the change. <br>Returns: The property value after the change. |
| `OldValue` | Gets the value of the property before the change. <br>Returns: The property value before the change. |
| `Property` | Gets the identifier for the dependency property where the value change occurred. <br>Returns: The identifier field of the dependency property where the value change occurred. |

## System.Windows.DependencyPropertyChangedEventHandler

Represents the method that will handle events raised when a `DependencyProperty` is changed on a particular `DependencyObject` implementation.

## System.Windows.Deployment

Provides application part and localization information in the application manifest when deploying a Silverlight-based application.

### Properties / fields

| Member | Description |
|---|---|
| `Current` | Gets the current `Deployment` object. <br>Returns: The current `Deployment` object. |
| `EntryPointAssembly` | Gets a string name that identifies which part in the `Deployment` is the entry point assembly. <br>Returns: A string that names the assembly that should be used as the entry point assembly. This is expected to be the name of one of the assemblies you specified as an `AssemblyPart`. |
| `EntryPointType` | Gets a string that identifies the namespace and type name of the class that contains the `Application` entry point for your application. <br>Returns: The namespace and type name of the class that contains the `Application` entry point. |
| `ExternalCallersFromCrossDomain` | Gets a value that indicates the level of access that cross-domain callers have to the Silverlight-based application in this deployment. <br>Returns: A value that indicates the access level of cross-domain callers. |
| `ExternalParts` | Gets a collection of `ExternalPart` instances that represent the external assemblies required by the application. <br>Returns: The collection of external assembly parts. The default is an empty collection. |
| `OutOfBrowserSettings` | Gets an object that contains information about the application that is used for out-of-browser support. <br>Returns: Information about the application that is used for out-of-browser support. |
| `Parts` | Gets a collection of assembly parts that are included in the deployment. <br>Returns: The collection of assembly parts. The default is an empty collection. |
| `RuntimeVersion` | Gets the Silverlight runtime version that this deployment supports. <br>Returns: The Silverlight runtime version that this deployment supports. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EntryPointAssemblyProperty` | Identifies the `EntryPointAssembly` dependency property. <br>Returns: The identifier for the `EntryPointAssembly` dependency property. |
| `EntryPointTypeProperty` | Identifies the `EntryPointType` dependency property. <br>Returns: The identifier for the `EntryPointType` dependency property. |
| `ExternalCallersFromCrossDomainProperty` | Identifies the `ExternalCallersFromCrossDomain` dependency property. <br>Returns: The identifier for the `ExternalCallersFromCrossDomain` dependency property. |
| `ExternalPartsProperty` | Identifies the `ExternalParts` dependency property. <br>Returns: The identifier for the `ExternalParts` dependency property. |
| `OutOfBrowserSettingsProperty` | Identifies the `OutOfBrowserSettings` dependency property. <br>Returns: The identifier for the `OutOfBrowserSettings` dependency property. |
| `PartsProperty` | Identifies the `Parts` dependency property. <br>Returns: The identifier for the `Parts` dependency property. |
| `RuntimeVersionProperty` | Identifies the `RuntimeVersion` dependency property. <br>Returns: The identifier for the `RuntimeVersion` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Deployment` class. |
| `GetAppIdForUri(Uri,String@)` | [SECURITY CRITICAL] Gets a value that represents a unique ID for an out-of-browser application. <br>`AppUri`: The absolute URI of the application's .xap file on its host server. <br>`xapLocationStr`: When this method returns, contains the local path to the .xap file in the offline application cache. This parameter is passed uninitialized. <br>Returns: A unique ID for the out-of-browser application. |
| `RegisterAssembly(Reflection.Assembly)` | [SECURITY CRITICAL] Enables a native Silverlight host, such as Expression Blend or Visual Studio, to instruct Silverlight to register an assembly that the Silverlight host has separately loaded into the host-managed application domain in which a Silverlight application is running. <br>`assembly`: The assembly that the Silverlight host has separately loaded. |
| `SetCurrentApplication(Application)` | [SECURITY CRITICAL] Allows a native host of the Silverlight plug-in to specify the current `Application` object of the running Silverlight application. <br>`application`: The `Application` object that the native host is setting as the current `Application`. |

## System.Windows.Documents.Block

An abstract class that provides a base for all block-level content elements.

### Properties / fields

| Member | Description |
|---|---|
| `LineHeight` | Gets or sets the height of each line of content. <br>Returns: The height of each line in pixels. A value of 0 indicates that the line height is determined automatically from the current font characteristics. The default is 0. |
| `LineStackingStrategy` | Gets or sets a value that indicates how a line box is determined for each line of text in a `Block`. <br>Returns: A value that indicates how a line box is determined for each line of text in a `Block`. The default is `MaxHeight`. |
| `TextAlignment` | Gets or sets the horizontal alignment of the text content. <br>Returns: The horizontal alignment of the text content. The default is `Left`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `LineHeightProperty` | Identifies the `LineHeight` dependency property. <br>Returns: Identifier for the `LineHeight` dependency property. |
| `LineStackingStrategyProperty` | Identifies the `LineStackingStrategy` dependency property. <br>Returns: Identifier for the `LineStackingStrategy` dependency property. |
| `TextAlignmentProperty` | Identifies the `TextAlignment` dependency property. <br>Returns: The identifier for the `TextAlignment` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Block` class. |

## System.Windows.Documents.BlockCollection

Represents a collection of `Block` elements.

## System.Windows.Documents.Bold

Provides an inline-level content element that causes content to render with a bold font weight.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Bold` class. |

## System.Windows.Documents.FontSource

Represents one or more fonts created from a stream.

### Methods

| Member | Description |
|---|---|
| `Constructor(IO.Stream)` | Initializes a new instance of the `FontSource` class. <br>`stream`: The stream that contains the font source. |
| `Constructor(Media.GlyphTypeface)` | Initializes a new instance of the `FontSource` class. <br>`glyphtypeface`: The `GlyphTypeface` object that contains the font file. |

## System.Windows.Documents.Glyphs

Provides a visual representation of letters, characters, or symbols, in a specific font and style.

### Properties / fields

| Member | Description |
|---|---|
| `Fill` | Gets or sets the `Brush` that is used to render the glyphs. <br>Returns: The brush to use to render the glyphs. The default is null, which is evaluated as `Transparent` for rendering. |
| `FontRenderingEmSize` | Gets or sets the font size used for rendering the glyphs. <br>Returns: The font size used for rendering the glyphs. The default is 0. |
| `FontSource` | Gets or sets the font source that is applied to the `Glyphs` for rendering content. <br>Returns: The font source used to render content in the `Glyphs`. The default is a null reference (Nothing in Visual Basic). |
| `FontUri` | Gets or sets the location of the font used for rendering the glyphs. <br>Returns: The URI source of the font. The default is null. |
| `Indices` | Gets or sets the glyph indices for the glyphs. <br>Returns: A string that defines glyph indices as well as other glyph specifics in a string mini-language. The default is null. |
| `OriginX` | Gets or sets the x origin for the glyphs. <br>Returns: The x origin of the `Glyphs`, in pixels. The default is 0. |
| `OriginY` | Gets or sets the y origin for the glyphs. <br>Returns: The y origin of the `Glyphs`, in pixels. The default is 0. |
| `StyleSimulations` | Gets or sets the style simulations applied to the glyphs. <br>Returns: One of the enumeration values that specifies the style simulations to apply to the glyphs. The default is None. |
| `UnicodeString` | Gets or sets the Unicode string to render in glyphs. <br>Returns: A Unicode string with XAML-compatible encoding. The default is an empty string. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FillProperty` | Identifies the `Fill` dependency property. <br>Returns: The identifier for the `Fill` dependency property. |
| `FontRenderingEmSizeProperty` | Identifies the `FontRenderingEmSize` dependency property. <br>Returns: The identifier for the `FontRenderingEmSize` dependency property. |
| `FontUriProperty` | Identifies the `FontUri` dependency property. <br>Returns: The identifier for the `FontUri` dependency property. |
| `IndicesProperty` | Identifies the `Indices` dependency property. <br>Returns: The identifier for the `Indices` dependency property. |
| `OriginXProperty` | Identifies the `OriginX` dependency property. <br>Returns: The identifier for the `OriginX` dependency property. |
| `OriginYProperty` | Identifies the `OriginY` dependency property. <br>Returns: The identifier for the `OriginY` dependency property. |
| `StyleSimulationsProperty` | Identifies the `StyleSimulations` dependency property. <br>Returns: The identifier for the `StyleSimulations` dependency property. |
| `UnicodeStringProperty` | Identifies the `UnicodeString` dependency property. <br>Returns: The identifier for the `UnicodeString` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Glyphs` class. |

## System.Windows.Documents.Hyperlink

Provides an inline-level content element that provides facilities for hosting hyperlinks.

### Properties / fields

| Member | Description |
|---|---|
| `Command` | Gets or sets a command to associate with the `Hyperlink`. <br>Returns: A command to associate with the `Hyperlink`. The default is null. |
| `CommandParameter` | Gets or sets command parameters associated with the command specified by the `Command` property. <br>Returns: An object specifying parameters for the command specified by the `Command` property. The default is null. |
| `MouseOverForeground` | Gets or sets the brush that paints the foreground color when the mouse pointer moves over the `Hyperlink`. <br>Returns: The brush that paints the foreground color when the mouse pointer moves over the `Hyperlink`. |
| `MouseOverTextDecorations` | Gets or sets the `TextDecorationCollection` that decorates the `Hyperlink`. <br>Returns: A `TextDecorationCollection` that decorates the `Hyperlink`. |
| `NavigateUri` | Gets or sets a URI to navigate to when the `Hyperlink` is activated. <br>Returns: The URI to navigate to when the `Hyperlink` is activated. The default is null. |
| `TargetName` | Gets or sets the name of a target window or frame for the `Hyperlink`. <br>Returns: A string that specifies the name of a target window or frame for the `Hyperlink`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CommandParameterProperty` | Identifies the `CommandParameter` dependency property. <br>Returns: The identifier for the `CommandParameter` dependency property. |
| `CommandProperty` | Identifies the `Command` dependency property. <br>Returns: Identifier for the `Command` dependency property. |
| `MouseOverForegroundProperty` | Identifies the `MouseOverForeground` dependency property. <br>Returns: Identifier for the `MouseOverForeground` dependency property. |
| `MouseOverTextDecorationsProperty` | Identifies the `MouseOverTextDecorations` dependency property. <br>Returns: Identifier for the `MouseOverTextDecorations` dependency property. |
| `NavigateUriProperty` | Identifies the `NavigateUri` dependency property. <br>Returns: Identifier for the `NavigateUri` dependency property. |
| `TargetNameProperty` | Identifies the `TargetName` dependency property. <br>Returns: Identifier for the `TargetName` dependency property. |

### Events

| Member | Description |
|---|---|
| `Click` | Occurs when the left mouse button is clicked on a `Hyperlink`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Hyperlink` class. |

## System.Windows.Documents.Inline

Provides a base for inline flow content element behavior.

### Properties / fields

| Member | Description |
|---|---|
| `TextDecorations` | Gets or sets a value that specifies the text decorations that are applied to the content in an `Inline` element. <br>Returns: A `TextDecorationCollection`, or null if no text decorations are applied. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FontFamilyProperty` | Identifies the `FontFamily` dependency property. <br>Returns: The identifier for the `FontFamily` dependency property. |
| `FontSizeProperty` | Identifies the `FontSize` dependency property. <br>Returns: The identifier for the `FontSize` dependency property. |
| `FontStretchProperty` | Identifies the `FontStretch` dependency property. <br>Returns: The identifier for the `FontStretch` dependency property. |
| `FontStyleProperty` | Identifies the `FontStyle` dependency property. <br>Returns: The identifier for the `FontStyle` dependency property. |
| `FontWeightProperty` | Identifies the `FontWeight` dependency property. <br>Returns: The identifier for the `FontWeight` dependency property. |
| `ForegroundProperty` | Identifies the `Foreground` dependency property. <br>Returns: The identifier for the `Foreground` dependency property. |
| `LanguageProperty` | Identifies the `Language` dependency property. <br>Returns: The identifier for the `Language` dependency property. |
| `TextDecorationsProperty` | Identifies the `TextDecorations` dependency property. <br>Returns: The identifier for the `TextDecorations` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Inline` class. |

## System.Windows.Documents.InlineCollection

Represents a collection of `Inline` elements.

### Methods

| Member | Description |
|---|---|
| `Add(String)` | Adds a string to the collection. <br>`text`: The text to add. |

## System.Windows.Documents.InlineUIContainer

Provides an inline content element that enables `UIElement` types to be embedded in content.

### Properties / fields

| Member | Description |
|---|---|
| `Child` | Gets or sets the `UIElement` hosted by the `InlineUIContainer`. <br>Returns: The `UIElement` hosted by the `InlineUIContainer`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `InlineUIContainer` class. |

## System.Windows.Documents.Italic

Provides an inline-level flow content element that causes content to render with an italic font style.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Italic` class. |

## System.Windows.Documents.LineBreak

Represents an inline element that causes a new line to begin in content when rendered in a text container.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LineBreak` class. |

## System.Windows.Documents.LogicalDirection

Specifies a logical direction in which to perform certain text operations, such as inserting, retrieving, or navigating through text relative to a specified position (a `TextPointer`).

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Backward` | Backward, or from right to left. |
| `Forward` | Forward, or from left to right. |

## System.Windows.Documents.Paragraph

Provides a block-level content element that is used to group content into a paragraph.

### Properties / fields

| Member | Description |
|---|---|
| `Inlines` | Gets an `InlineCollection` containing the top-level `Inline` elements that include the contents of the `Paragraph`. <br>Returns: An `InlineCollection` containing the `Inline` elements that include the contents of the `Paragraph`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Paragraph` class. |

## System.Windows.Documents.Run

Represents a discrete section of formatted or unformatted text.

### Properties / fields

| Member | Description |
|---|---|
| `FlowDirection` | Gets or sets the direction that text and other user interface elements flow within the `Run` element that controls their layout. <br>Returns: The direction that text and other UI elements flow within the `Run` element. The default value is `LeftToRight`. |
| `Text` | Gets or sets the text contents of the `Run`. <br>Returns: A string that specifies the text contents of the `Run`. The default is `Empty`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FlowDirectionProperty` | Identifies the `FlowDirection` dependency property. <br>Returns: The `FlowDirection` dependency property identifier. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Run` class. |

## System.Windows.Documents.Section

A block-level element used for grouping other `Block` elements.

### Properties / fields

| Member | Description |
|---|---|
| `Blocks` | Gets a `BlockCollection` containing the top-level `Block` elements that comprise the contents of the `Section`. <br>Returns: A `BlockCollection` containing the `Block` elements that comprise the contents of the `Section`. This property has no default value. |
| `HasTrailingParagraphBreakOnPaste` | Gets or sets a value that indicates whether a trailing paragraph break should be inserted after the last paragraph when copying the contents of a root `Section` element to the clipboard. <br>Returns: true if a trailing paragraph break should be included; otherwise false. |

## System.Windows.Documents.Span

Groups other `Inline` content elements.

### Properties / fields

| Member | Description |
|---|---|
| `Inlines` | Gets an `InlineCollection` containing the top-level inline elements that include the contents of `Span`. <br>Returns: An `InlineCollection` containing the inline elements that include the contents of the `Span`. This property has no default value. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Span` class. |

## System.Windows.Documents.TextElement

An abstract class used as the base class for the abstract `Block` and `Inline` classes.

### Properties / fields

| Member | Description |
|---|---|
| `CharacterSpacing` | Gets or sets the distance between characters of text in the control measured in 1000ths of the font size. <br>Returns: The distance between characters of text in the control measured in 1000ths of the font size. The default is 0. |
| `ContentEnd` | Gets a `TextPointer` that represents the end of the content in the element. <br>Returns: A `TextPointer` that represents the end of the content in the `TextElement`. |
| `ContentStart` | Gets a `TextPointer` that represents the start of content in the element. <br>Returns: A `TextPointer` that represents the start of the content in the `TextElement`. |
| `ElementEnd` | Gets a `TextPointer` that represents the position just after the end of the element. <br>Returns: A `TextPointer` that represents the position just after the end of the `TextElement`. |
| `ElementStart` | Gets a `TextPointer` that represents the position just before the start of the element. <br>Returns: A `TextPointer` that represents the position just before the start of the `TextElement`. |
| `FontFamily` | Gets or sets the preferred top-level font family for the content of the element. <br>Returns: The preferred font family, or a primary preferred font family with one or more fallback font families. See `FontFamily` for default information. |
| `FontSize` | Gets or sets the font size for the content of the element. <br>Returns: The desired font size in pixels. The default is 11 pixels. |
| `FontStretch` | Gets or sets the glyph width of the font in a family to select. <br>Returns: One of the `FontStretches` property values, specifying the desired font stretch. The default is `Normal`. |
| `FontStyle` | Gets or sets the font style for the content in this element. <br>Returns: One of the `FontStyles` property values, specifying the font style. The default is `Normal`. |
| `FontWeight` | Gets or sets the top-level font weight to select from the font family for the content in this element. <br>Returns: One of the `FontWeights` property values, specifying the font weight. The default is `Normal`. |
| `Foreground` | Gets or sets the `Brush` to apply to the content in this element. <br>Returns: The brush that is applied to the text contents. The default is a `SolidColorBrush` with `Color` value `Black`. |
| `Language` | Gets or sets the language of the content within an `TextElement` element. <br>Returns: A value of type `XmlLanguage`. The default is a `XmlLanguage` value created with the string en-US (`IetfLanguageTag` is "en-US"). |
| `Name` | Gets or sets a unique identification for the object. `Name` can only be set from initial parsing of XAML. <br>Returns: The unique identifier for the object. This property is read-only for code, but write-only for XAML, due to special parser handling of XAML properties named "Name". |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CharacterSpacingProperty` | Identifies the `CharacterSpacing`dependency property. <br>Returns: The identifier for the `CharacterSpacing` dependency property. |
| `FontFamilyProperty` | Identifies the `FontFamily` dependency property. <br>Returns: The identifier for the `FontFamily` dependency property. |
| `FontSizeProperty` | Identifies the `FontSize` dependency property. <br>Returns: Identifier for the `FontSize` dependency property. |
| `FontStretchProperty` | Identifies the `FontStretch` dependency property. <br>Returns: Identifier for the `FontStretch` dependency property. |
| `FontStyleProperty` | Identifies the `FontStyle` dependency property. <br>Returns: Identifier for the `FontStyle` dependency property. |
| `FontWeightProperty` | Identifies the `FontWeight` dependency property. <br>Returns: Identifier for the `FontWeight` dependency property. |
| `ForegroundProperty` | Identifies the `Foreground` dependency property. <br>Returns: Identifier for the `Foreground` dependency property. |
| `LanguageProperty` | Identifies the `Language` dependency property. <br>Returns: Identifier for the `Language` dependency property. |

### Methods

| Member | Description |
|---|---|
| `FindName(String)` | Gets an object in the Silverlight object model by referencing the object's x:Name or `Name` attribute value. <br>`name`: The name of the object to retrieve. <br>Returns: The object that has the specified name, or null if no object is retrieved. |
| `OnCreateAutomationPeer` | Returns a class-specific `AutomationPeer` implementation for use by the Silverlight automation infrastructure. <br>Returns: A type-specific `AutomationPeer` for the `TextElement` object. |

## System.Windows.Documents.TextElementCollection`1

Provides standard facilities for creating and managing a type-safe, ordered collection of `TextElement` objects.

## System.Windows.Documents.TextPointer

Represents a position within a `RichTextBox`.

### Properties / fields

| Member | Description |
|---|---|
| `IsAtInsertionPosition` | Gets a value that indicates whether the current position is an insertion. <br>Returns: true if the current position is an insertion position; otherwise, false. |
| `LogicalDirection` | Gets the logical direction associated with the current position, which is used to disambiguate content associated with the current position. <br>Returns: The `LogicalDirection` value that is associated with the current position. |
| `Parent` | Gets the logical parent that contains the current position. <br>Returns: The logical parent that contains the current position. Can return the `RichTextBox` when at the top of the content stack. |

### Methods

| Member | Description |
|---|---|
| `CompareTo(Documents.TextPointer)` | Performs an ordinal comparison between the positions specified by the current `TextPointer` and a second specified `TextPointer`. <br>`position`: A `TextPointer` that specifies a position to compare to the current position. <br>Returns: -1 if the current `TextPointer` precedes position; 0 if the locations are the same; +1 if the current `TextPointer` follows positions. |
| `GetCharacterRect(Documents.LogicalDirection)` | Returns a bounding box for content that borders the current `TextPointer` in the specified logical direction. <br>`direction`: One of the `LogicalDirection` values that specify the logical direction in which to find a content bounding box. <br>Returns: A `Rect` for content that borders the current `TextPointer` in the specified direction, or `Empty` if current and valid layout information is unavailable. |
| `GetNextInsertionPosition(Documents.LogicalDirection)` | Returns a `TextPointer` to the next insertion position in the specified logical direction. <br>`direction`: One of the `LogicalDirection` values that specify the logical direction in which to search for the next insertion position. <br>Returns: A `TextPointer` that identifies the next insertion position in the requested direction, or null if no next insertion position can be found. |
| `GetPositionAtOffset(Int32,Documents.LogicalDirection)` | Returns a `TextPointer` to the position indicated by the specified offset, in symbols, from the beginning of the current `TextPointer` and in the specified direction. <br>`offset`: An offset, in symbols, for which to calculate and return the position. If the offset is negative, the returned `TextPointer` precedes the current `TextPointer`; otherwise, it follows. <br>`direction`: One of the `LogicalDirection` values that specifies the logical direction of the returned `TextPointer`. <br>Returns: A `TextPointer` to the position indicated by the specified offset and in the direction specified by the direction parameter, or null if the offset extends past the end of the content. |

## System.Windows.Documents.TextSelection

Encapsulates the selection state for the `RichTextBox` control.

### Properties / fields

| Member | Description |
|---|---|
| `End` | Gets a `TextPointer` that represents the end of the current selection. <br>Returns: A `TextPointer` that represents the end of the current selection. |
| `Start` | Gets a `TextPointer` that represents the beginning of the current selection. <br>Returns: A `TextPointer` that represents the beginning of the current selection. |
| `Text` | Gets or sets the plain text contents of the current selection. <br>Returns: A string that contains the plain text contents of the current selection. |
| `Xaml` | Gets or sets the XAML representation of the current selection. <br>Returns: A `String` that is a XAML representation of the current selection. This XAML representation is the same XAML that is applied to the clipboard for a copy operation. |

### Methods

| Member | Description |
|---|---|
| `ApplyPropertyValue(DependencyProperty,Object)` | Applies the specified formatting property and value to the current selection. <br>`formattingProperty`: A formatting property to apply. <br>`value`: The value for the formatting property. |
| `GetPropertyValue(DependencyProperty)` | Gets the value of the specified formatting property on the current selection. <br>`formattingProperty`: A formatting property to get the value of on the current selection. <br>Returns: An object that indicates the value of the specified formatting property on the current selection. |
| `Insert(Documents.TextElement)` | Inserts or replaces the content at the current selection as a `TextElement`. <br>`element`: The `TextElement` to be inserted. |
| `Select(Documents.TextPointer,Documents.TextPointer)` | Updates the current selection, taking two `TextPointer` positions to indicate the updated selection. <br>`anchorPosition`: A fixed anchor position that marks one end of the updated selection. <br>`movingPosition`: A movable position that marks the other end of the updated selection. |

## System.Windows.Documents.Underline

Provides an inline-level content element that causes content to render with an underlined text decoration.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Underline` class. |

## System.Windows.DragEventArgs

Provides data for drag-and-drop events in Silverlight.

### Properties / fields

| Member | Description |
|---|---|
| `Data` | Gets a data object (implements `IDataObject`) that contains the data associated with the corresponding drag event. This value is not useful in all event cases; see Remarks. <br>Returns: The data object that contains the data that is associated with the corresponding drag event. |
| `Handled` | Gets or sets a value that indicates the present state of the event handling for a routed event as it travels the route. <br>Returns: true if the event is marked handled; otherwise, false. The default value is false. |

### Methods

| Member | Description |
|---|---|
| `GetPosition(UIElement)` | Returns a drop point that is relative to a specified `UIElement`. <br>`relativeTo`: The `UIElement` for which to get a relative drop point. <br>Returns: A drop point that is relative to the element specified in . |

## System.Windows.DragEventHandler

Represents the method that will handle the `DragEnter`, `DragLeave`, `DragOver`, and `Drop` events of a `UIElement`.

## System.Windows.Duration

Represents the duration of time that a `Timeline` is active.

### Properties / fields

| Member | Description |
|---|---|
| `Automatic` | Gets a `Duration` value that is automatically determined. <br>Returns: A `Duration` initialized to an automatic value. |
| `Forever` | Gets a `Duration` value that represents an infinite interval. <br>Returns: A `Duration` initialized to a forever value. |
| `HasTimeSpan` | Gets a value that indicates if this `Duration` represents a `TimeSpan` value. <br>Returns: true if this `Duration` is a `TimeSpan` value; otherwise, false. |
| `TimeSpan` | Gets the `TimeSpan` value that this `Duration` represents. <br>Returns: The `TimeSpan` value that this `Duration` represents. |

### Methods

| Member | Description |
|---|---|
| `Constructor(TimeSpan)` | Initializes a new instance of the `Duration` structure with the supplied `TimeSpan` value. <br>`timeSpan`: Represents the initial time interval of this duration. |
| `Add(Duration)` | Adds the value of the specified `Duration` to this `Duration`. <br>`duration`: An instance of `Duration` that represents the value of the current instance plus `duration`. <br>Returns: If each involved `Duration` has values, a `Duration` that represents the combined values. Otherwise this method returns null. |
| `Compare(Duration,Duration)` | Compares one `Duration` value to another. <br>`t1`: The first instance of `Duration` to compare. <br>`t2`: The second instance of `Duration` to compare. <br>Returns: If is less than , a negative value that represents the difference. If is equal to , a value of 0. If is greater than , a positive value that represents the difference. |
| `Equals(Object)` | Determines whether a specified object is equal to a `Duration`. <br>`value`: Object to check for equality. <br>Returns: true if value is equal to this `Duration`; otherwise, false. |
| `Equals(Duration)` | Determines whether a specified `Duration` is equal to this `Duration`. <br>`duration`: The `Duration` to check for equality. <br>Returns: true if `duration` is equal to this `Duration`; otherwise, false. |
| `Equals(Duration,Duration)` | Determines whether two `Duration` values are equal. <br>`t1`: First `Duration` to compare. <br>`t2`: Second `Duration` to compare. <br>Returns: true if is equal to ; otherwise, false. |
| `GetHashCode` | Gets a hash code for this object. <br>Returns: The hash code identifier. |
| `Plus(Duration)` | Adds one `Duration` to this `Duration`. <br>`duration`: The `Duration` to add. <br>Returns: The summed `Duration`. |
| `Subtract(Duration)` | Subtracts the specified `Duration` from this `Duration`. <br>`duration`: The `Duration` to subtract from this `Duration`. <br>Returns: The subtracted `Duration`. |
| `ToString` | Converts a `Duration` to a `String` representation. <br>Returns: A `String` representation of this `Duration`. |
| `op_Addition(Duration,Duration)` | Adds two `Duration` values together. <br>`t1`: The first `Duration` to add. <br>`t2`: The second `Duration` to add. <br>Returns: If both `Duration` values have `TimeSpan` values, this method returns the sum of those two values. If either value is set to `Automatic`, the method returns `Automatic`. If either value is set to `Forever`, the method returns `Forever`.If either `t1` or `t2` has no value, this method returns null. |
| `op_Equality(Duration,Duration)` | Determines whether two `Duration` cases are equal. <br>`t1`: The first `Duration` to compare. <br>`t2`: The second `Duration` to compare. <br>Returns: true if both `Duration` values have equal property values, or if all `Duration` values are null. Otherwise, this method returns false. |
| `op_GreaterThan(Duration,Duration)` | Determines if one `Duration` is greater than another. <br>`t1`: The `Duration` value to compare. <br>`t2`: The second `Duration` value to compare. <br>Returns: true if both and have values and is greater than ; otherwise, false. |
| `op_GreaterThanOrEqual(Duration,Duration)` | Determines whether a `Duration` is greater than or equal to another. <br>`t1`: The first instance of `Duration` to compare. <br>`t2`: The second instance of `Duration` to compare. <br>Returns: true if both and have values and is greater than or equal to ; otherwise, false. |
| `op_Implicit(TimeSpan)` | Implicitly creates a `Duration` from a given `TimeSpan`. <br>`timeSpan`: `TimeSpan` from which a `Duration` is implicitly created. <br>Returns: A created `Duration`. |
| `op_Inequality(Duration,Duration)` | Determines if two `Duration` cases are not equal. <br>`t1`: The first `Duration` to compare. <br>`t2`: The second `Duration` to compare. <br>Returns: true if exactly one of or represent a value, or if they both represent values that are not equal; otherwise, false. |
| `op_LessThan(Duration,Duration)` | Determines if a `Duration` is less than the value of another instance. <br>`t1`: The first `Duration` to compare. <br>`t2`: The second `Duration` to compare. <br>Returns: true if both and have values and is less than ; otherwise, false. |
| `op_LessThanOrEqual(Duration,Duration)` | Determines if a `Duration` is less than or equal to another. <br>`t1`: The `Duration` to compare. <br>`t2`: The `Duration` to compare. <br>Returns: true if both and have values and is less than or equal to ; otherwise, false. |
| `op_Subtraction(Duration,Duration)` | Subtracts the value of one `Duration` from another. <br>`t1`: The first `Duration`. <br>`t2`: The `Duration` to subtract. <br>Returns: If each `Duration` has values, a `Duration` that represents the value of `t1` minus `t2`. If `t1` has a value of `Forever` and `t2` has a value of `TimeSpan`, this method returns `Forever`. Otherwise this method returns null. |
| `op_UnaryPlus(Duration)` | Returns the specified `Duration`. <br>`duration`: The `Duration` to get. <br>Returns: The `Duration` operation result. |

## System.Windows.DurationConverter

Provides type conversion support for the `Duration` structure.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DurationConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Returns whether this converter can convert an object of one type to the `Duration` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: A `Type` that represents the type you want to convert from. <br>Returns: true if this converter can perform the conversion; otherwise, false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the given value to the `Duration` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: The `CultureInfo` to use as the current culture. <br>`value`: The object to convert. <br>Returns: The returned `Duration`. |

## System.Windows.ElevatedPermissions

Defines constants that indicate whether elevated permissions are required for an out-of-browser application.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `NotRequired` | Elevated permissions are not required to run the application outside the browser. |
| `Required` | Elevated permissions are required to run the application outside the browser. |

## System.Windows.EventTrigger

Represents a trigger that applies a set of actions (animation storyboards) in response to an event.

### Properties / fields

| Member | Description |
|---|---|
| `Actions` | Gets the collection of `BeginStoryboard` objects that this `EventTrigger` maintains. <br>Returns: The existing `TriggerActionCollection`. |
| `RoutedEvent` | Gets or sets the name of the event that initiates the trigger. <br>Returns: The name or identifier of the event. See Remarks. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EventTrigger` class. |

## System.Windows.ExceptionRoutedEventArgs

Provides event data for exceptions that are raised as events by asynchronous operations, such as `MediaFailed` or `ImageFailed`.

### Properties / fields

| Member | Description |
|---|---|
| `ErrorException` | Gets the underlying exception or native-level error reported by the event. <br>Returns: The exception being reported. |

## System.Windows.ExceptionRoutedEventHandler

Represents the method that will handle certain events that report exceptions. These exceptions generally come from asynchronous operations where holding up the UI thread for exception checking is not an option.

## System.Windows.Expression

This type is not intended to be used from your code or to be derived from.

## System.Windows.ExtensionPart

Represents a zip file containing assemblies used by application library caching.

### Properties / fields

| Member | Description |
|---|---|
| `Source` | Gets or sets the file name or URI of the external library package. <br>Returns: The file name or URI of the external library package. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ExtensionPart` class. |

## System.Windows.ExternalPart

Defines a base class for specifying parts of a Silverlight application that are external to the application package (.xap file).

### Properties / fields

| Member | Description |
|---|---|
| `Source` | Gets or sets the URI of the external part. <br>Returns: The URI of the external part. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ExternalPart` class. |

## System.Windows.ExternalPartCollection

Represents a collection of `ExternalPart` instances that indicate parts of a Silverlight application that are external to the application package (.xap file).

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ExternalPartCollection` class. |

## System.Windows.FlowDirection

Defines constants that specify the content flow direction for text and UI elements.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `LeftToRight` | Indicates that content should flow from left to right. |
| `RightToLeft` | Indicates that content should flow from right to left. |

## System.Windows.FontStretch

Describes the degree to which a font has been stretched, compared to the normal aspect ratio of that font.

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Compares an object with the current `FontStretch` object. <br>`obj`: The instance of the object to compare for equality. <br>Returns: true if two instances are equal; otherwise, false. |
| `GetHashCode` | Retrieves the hash code for this object. <br>Returns: An integer hash value. |
| `ToString` | Creates a `String` representation of the current `FontStretch` object. <br>Returns: A `String` value representation of the object. |
| `op_Equality(FontStretch,FontStretch)` | Compares two instances of `FontStretch` for equality. <br>`left`: First instance of `FontStretch` to compare. <br>`right`: Second instance of `FontStretch` to compare. <br>Returns: true when the specified `FontStretch` objects are equal; otherwise, false. |
| `op_Inequality(FontStretch,FontStretch)` | Evaluates two instances of `FontStretch` to determine inequality. <br>`left`: The first instance of `FontStretch` to compare. <br>`right`: The second instance of `FontStretch` to compare. <br>Returns: false if is equal to ; otherwise, true. |

## System.Windows.FontStretches

Provides a set of predefined font stretches as static property values.

### Properties / fields

| Member | Description |
|---|---|
| `Condensed` | Specifies a condensed font stretch. <br>Returns: A value that represents a condensed font stretch. |
| `Expanded` | Specifies an expanded font stretch. <br>Returns: A value that represents an expanded font stretch. |
| `ExtraCondensed` | Specifies an extra-condensed font stretch. <br>Returns: A value that represents an extra-condensed font stretch. |
| `ExtraExpanded` | Specifies an extra-expanded font stretch. <br>Returns: A value that represents an extra-expanded font stretch. |
| `Normal` | Specifies a normal font stretch. <br>Returns: A value that represents a normal font stretch. |
| `SemiCondensed` | Specifies a semi-condensed font stretch. <br>Returns: A value that represents a semi-condensed font stretch. |
| `SemiExpanded` | Specifies a semi-expanded font stretch. <br>Returns: A value that represents a semi-expanded font stretch. |
| `UltraCondensed` | Specifies an ultra-condensed font stretch. <br>Returns: A value that represents an ultra-condensed font stretch. |
| `UltraExpanded` | Specifies an ultra-expanded font stretch. <br>Returns: A value that represents an ultra-expanded font stretch. |

## System.Windows.FontStyle

Represents the style of a font face (for instance, as normal or italic).

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Compares an object with the current `FontStyle` instance for equality. <br>`obj`: An object that represents the `FontStyle` to compare for equality. <br>Returns: true to show the two instances are equal; otherwise, false. |
| `GetHashCode` | Retrieves the hash code for this object. <br>Returns: An integer hash value. |
| `ToString` | Creates a string that represents the current `FontStyle` object. <br>Returns: A string that represents the value of the `FontStyle` object. |
| `op_Equality(FontStyle,FontStyle)` | Compares two instances of `FontStyle` for equality. <br>`left`: The first instance of `FontStyle` to compare. <br>`right`: The second instance of `FontStyle` to compare. <br>Returns: true if the specified `FontStyle` objects are equal; otherwise, false. |
| `op_Inequality(FontStyle,FontStyle)` | Evaluates two instances of `FontStyle` to determine inequality. <br>`left`: The first instance of `FontStyle` to compare. <br>`right`: The second instance of `FontStyle` to compare. <br>Returns: false to show is equal to ; otherwise, true. |

## System.Windows.FontStyles

Provides a set of predefined font styles as static property values.

### Properties / fields

| Member | Description |
|---|---|
| `Italic` | Specifies an italic font style. <br>Returns: A font style that represents an italic font style. |
| `Normal` | Specifies a normal, or roman, font style. <br>Returns: A font style that represents a normal, or roman, font style. |

## System.Windows.FontWeight

Refers to the density of a typeface, in terms of the lightness or heaviness of the strokes.

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Determines whether the current `FontWeight` object is equal to a specified object. <br>`obj`: The object to compare for equality. <br>Returns: true if the values are equal; otherwise, false. |
| `GetHashCode` | Retrieves the hash code for this object. <br>Returns: An integer hash value. |
| `ToString` | Returns a text string that represents the value of the `FontWeight` object. <br>Returns: A string that represents the value of the `FontWeight` object, such as "Light" or "Normal". |
| `op_Equality(FontWeight,FontWeight)` | Compares two instances of `FontWeight` for equality. <br>`left`: The first instance of `FontWeight` to compare. <br>`right`: The second instance of `FontWeight` to compare. <br>Returns: true if the values of `FontWeight` are equal; otherwise, false. |
| `op_Inequality(FontWeight,FontWeight)` | Evaluates two instances of `FontWeight` to determine inequality. <br>`left`: The first instance of `FontWeight` to compare. <br>`right`: The second instance of `FontWeight` to compare. <br>Returns: false if values of are equal to ; otherwise, true. |

## System.Windows.FontWeights

Provides a set of predefined font weights as static property values.

### Properties / fields

| Member | Description |
|---|---|
| `Black` | Specifies a "Black" font weight. <br>Returns: A `FontWeight` value that represents a "Black" font weight. |
| `Bold` | Specifies a "Bold" font weight. <br>Returns: A `FontWeight` value that represents a "Bold" font weight. |
| `ExtraBlack` | Specifies an "ExtraBlack" font weight. <br>Returns: A `FontWeight` value that represents an "ExtraBlack" font weight. |
| `ExtraBold` | Specifies an "ExtraBold" font weight. <br>Returns: A `FontWeight` value that represents an "ExtraBlack" font weight. |
| `ExtraLight` | Specifies an "ExtraLight" font weight. <br>Returns: A `FontWeight` value that represents an "ExtraLight" font weight. |
| `Light` | Specifies a "Light" font weight. <br>Returns: A `FontWeight` value that represents a "Light" font weight. |
| `Medium` | Specifies a "Medium" font weight. <br>Returns: A `FontWeight` value that represents a "Medium" font weight. |
| `Normal` | Specifies a "Normal" font weight. <br>Returns: A `FontWeight` value that represents a "Normal" font weight. |
| `SemiBold` | Specifies a "SemiBold" font weight. <br>Returns: A `FontWeight` value that represents a "SemiBold" font weight. |
| `Thin` | Specifies a "Thin" font weight. <br>Returns: A `FontWeight` value that represents a "Thin" font weight. |

## System.Windows.FrameworkElement

Provides a framework of common APIs for objects that participate in Silverlight layout. `FrameworkElement` also defines APIs related to data binding, object tree, and object lifetime feature areas in Silverlight.

### Properties / fields

| Member | Description |
|---|---|
| `ActualHeight` | Gets the rendered height of a `FrameworkElement`. <br>Returns: The height, in pixels, of the object. The default is 0. The default might be encountered if the object has not been loaded and undergone a layout pass. |
| `ActualWidth` | Gets the rendered width of a `FrameworkElement`. <br>Returns: The width, in pixels, of the object. The default is 0. The default might be encountered if the object has not been loaded and undergone a layout pass. |
| `Cursor` | Gets or sets the cursor image that displays while the mouse pointer is over a `FrameworkElement`. <br>Returns: The cursor image to display. The default is defined as null for code access. However, the appearance of the cursor image in UI at run time will come from a variety of factors. |
| `DataContext` | Gets or sets the data context for a `FrameworkElement` when it participates in data binding. <br>Returns: The object to use as data context. |
| `FlowDirection` | Gets or sets the direction that text and other user interface elements flow within any parent element that controls their layout. <br>Returns: The direction that text and other UI elements flow within their parent element, as a value of the enumeration. The default value is `LeftToRight`. |
| `Height` | Gets or sets the suggested height of a `FrameworkElement`. <br>Returns: The height, in pixels, of the object. The default is `NaN`. Except for the special `NaN` value, this value must be equal to or greater than 0. See Remarks for upper-bound information. |
| `HorizontalAlignment` | Gets or sets the horizontal alignment characteristics that are applied to a `FrameworkElement` when it is composed in a layout parent, such as a panel or items control. <br>Returns: A horizontal alignment setting, as a value of the enumeration. The default is `Stretch`. |
| `Language` | Gets or sets localization/globalization language information that applies to a `FrameworkElement`. <br>Returns: The language information for this object. The default is an `XmlLanguage` object that has its `IetfLanguageTag` value set to the string "en-US". |
| `Margin` | Gets or sets the outer margin of a `FrameworkElement`. <br>Returns: Provides margin values for the object. The default value is a default `Thickness` with all properties (dimensions) equal to 0. |
| `MaxHeight` | Gets or sets the maximum height constraint of a `FrameworkElement`. <br>Returns: The maximum height of the object, in pixels. The default value is `PositiveInfinity`. This value can be any value equal to or greater than 0. `PositiveInfinity` is also valid. |
| `MaxWidth` | Gets or sets the maximum width constraint of a `FrameworkElement`. <br>Returns: The maximum width of the object, in pixels. The default is `PositiveInfinity`. This value can be any value equal to or greater than 0. `PositiveInfinity` is also valid. |
| `MinHeight` | Gets or sets the minimum height constraint of a `FrameworkElement`. <br>Returns: The minimum height of the object, in pixels. The default is 0. This value can be any value equal to or greater than 0. However, `PositiveInfinity` is not valid. |
| `MinWidth` | Gets or sets the minimum width constraint of a `FrameworkElement`. <br>Returns: The minimum width of the object, in pixels. The default is 0. This value can be any value equal to or greater than 0. However, `PositiveInfinity` is not valid. |
| `Name` | Gets or sets the identifying name of the object. When a XAML processor creates the object tree from XAML markup, run-time code can refer to the XAML-declared object by this name. <br>Returns: The name of the object, which must be a string that is valid in the XamlName Grammar. The default is an empty string. |
| `Parent` | Gets the parent object of this `FrameworkElement` in the object tree. <br>Returns: The parent object of this object in the object tree. |
| `Resources` | Gets the locally defined resource dictionary. In XAML, you can establish resource items as child object elements of a frameworkElement.Resources property element, through XAML implicit collection syntax. <br>Returns: The current locally defined dictionary of resources, where each resource can be accessed by its key. |
| `Style` | Gets or sets an instance `Style` that is applied for this object during rendering. <br>Returns: The applied style for the object, if present; otherwise, null. The default for a default-constructed `FrameworkElement` is null. |
| `Tag` | Gets or sets an arbitrary object value that can be used to store custom information about this object. <br>Returns: The intended value. This property has no default value. |
| `Triggers` | Gets the collection of triggers for animations that are defined for a `FrameworkElement`. <br>Returns: The collection of triggers for animations that are defined for this object. |
| `VerticalAlignment` | Gets or sets the vertical alignment characteristics that are applied to a `FrameworkElement` when it is composed in a parent object such as a panel or items control. <br>Returns: A vertical alignment setting. The default is `Stretch`. |
| `Width` | Gets or sets the width of a `FrameworkElement`. <br>Returns: The width of the object, in pixels. The default is `NaN`. Except for the special `NaN` value, this value must be equal to or greater than 0. See Remarks for upper bound information. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ActualHeightProperty` | Identifies the `ActualHeight` dependency property. <br>Returns: The identifier for the `ActualHeight` dependency property. |
| `ActualWidthProperty` | Identifies the `ActualWidth` dependency property. <br>Returns: The identifier for the `ActualWidth` dependency property. |
| `CursorProperty` | Identifies the `Cursor` dependency property. <br>Returns: The identifier for the `Cursor` dependency property. |
| `DataContextProperty` | Identifies the `DataContext` dependency property. <br>Returns: The `DataContext` dependency property identifier. |
| `FlowDirectionProperty` | Identifies the `FlowDirection` dependency property. <br>Returns: The `FlowDirection` dependency property identifier. |
| `HeightProperty` | Identifies the `Height` dependency property. <br>Returns: The identifier for the `Height` dependency property. |
| `HorizontalAlignmentProperty` | Identifies the `HorizontalAlignment` dependency property. <br>Returns: The `HorizontalAlignment` dependency property identifier. |
| `LanguageProperty` | Identifies the `Language` dependency property. <br>Returns: The `Language` dependency property identifier. |
| `LoadedEvent` | Identifies the `Loaded` routed event. <br>Returns: The identifier for the `Loaded` routed event. |
| `MarginProperty` | Identifies the `Margin` dependency property. <br>Returns: The `Margin` dependency property identifier. |
| `MaxHeightProperty` | Identifies the `MaxHeight` dependency property. <br>Returns: The identifier for the `MaxHeight` dependency property. |
| `MaxWidthProperty` | Identifies the `MaxWidth` dependency property. <br>Returns: The identifier for the `MaxWidth` dependency property. |
| `MinHeightProperty` | Identifies the `MinHeight` dependency property. <br>Returns: The identifier for the `MinHeight` dependency property. |
| `MinWidthProperty` | Identifies the `MinWidth` dependency property. <br>Returns: The identifier for the `MinWidth` dependency property. |
| `NameProperty` | Identifies the `Name` dependency property. <br>Returns: The identifier for the `Name` dependency property. |
| `StyleProperty` | Identifies the `Style` dependency property. <br>Returns: The identifier for the `Style` dependency property. |
| `TagProperty` | Identifies the `Tag` dependency property. <br>Returns: The identifier for the `Tag` dependency property. |
| `VerticalAlignmentProperty` | Identifies the `VerticalAlignment` dependency property. <br>Returns: The `VerticalAlignment` dependency property identifier. |
| `WidthProperty` | Identifies the `Width` dependency property. <br>Returns: The identifier for the `Width` dependency property. |

### Events

| Member | Description |
|---|---|
| `BindingValidationError` | Occurs when a data validation error is reported by a binding source. |
| `LayoutUpdated` | Occurs when the layout of the Silverlight visual tree changes. |
| `Loaded` | Occurs when a `FrameworkElement` has been constructed and added to the object tree. |
| `SizeChanged` | Occurs when either the `ActualHeight` or the `ActualWidth` properties change value on a `FrameworkElement`. |
| `Unloaded` | Occurs when this object is no longer connected to the main object tree. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `FrameworkElement` class. |
| `ArrangeOverride(Size)` | Provides the behavior for the Arrange pass of Silverlight layout. Classes can override this method to define their own Arrange pass behavior. <br>`finalSize`: The final area within the parent that this object should use to arrange itself and its children. <br>Returns: The actual size that is used after the element is arranged in layout. |
| `FindName(String)` | Retrieves an object that has the specified identifier name. <br>`name`: The name of the requested object. <br>Returns: The requested object. This can be null if no matching object was found in the current XAML namescope. |
| `GetBindingExpression(DependencyProperty)` | Retrieves the `BindingExpression` for a dependency property where a binding is established. <br>`dp`: The dependency property identifier for the specific property on this `FrameworkElement` where you want to obtain the `BindingExpression`. <br>Returns: A `BindingExpression` for the binding, if the local value represented a data-bound value. May return null if the property is not a data-bound value. |
| `MeasureOverride(Size)` | Provides the behavior for the Measure pass of Silverlight layout. Classes can override this method to define their own Measure pass behavior. <br>`availableSize`: The available size that this object can give to child objects. Infinity (`PositiveInfinity`) can be specified as a value to indicate that the object will size to whatever content is available. <br>Returns: The size that this object determines it needs during layout, based on its calculations of the allocated sizes for child objects; or based on other considerations, such as a fixed container size. |
| `OnApplyTemplate` | When overridden in a derived class, is invoked whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. In simplest terms, this means the method is called just before a UI element displays in an application. For more information, see Remarks. |
| `SetBinding(DependencyProperty,Data.Binding)` | Attaches a binding to a `FrameworkElement`, using the provided binding object, and returns a `BindingExpressionBase` for possible later use. <br>`dp`: The dependency property identifier of the property that is data bound. <br>`binding`: The binding to use for the property. <br>Returns: A `BindingExpressionBase` object. See Remarks. |

## System.Windows.FrameworkTemplate

Creates an element tree of elements.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `FrameworkTemplate` class. |

## System.Windows.GpuInformation

Represents details for graphics processor unit (GPU) information as reported by the system of the local computer.

### Properties / fields

| Member | Description |
|---|---|
| `DeviceId` | Gets the device ID of the GPU. <br>Returns: The device ID for the GPU. |
| `DriverVersion` | Gets the driver version of the GPU. <br>Returns: The driver version of the GPU. |
| `VendorId` | Gets the vendor ID of the GPU. <br>Returns: The vendor ID of the GPU. |

## System.Windows.GridLength

Represents the length of elements that explicitly support `Star` unit types.

### Properties / fields

| Member | Description |
|---|---|
| `Auto` | Gets an instance of `GridLength` that holds a value whose size is determined by the size properties of the content object. <br>Returns: A instance of `GridLength` whose `GridUnitType` property is set to `Auto`. |
| `GridUnitType` | Gets the associated `GridUnitType` for the `GridLength`. <br>Returns: One of the `GridUnitType` values. The default is `Auto`. |
| `IsAbsolute` | Gets a value that indicates whether the `GridLength` holds a value that is expressed in pixels. <br>Returns: true if the `GridUnitType` property is `Pixel`; otherwise, false. |
| `IsAuto` | Gets a value that indicates whether the `GridLength` holds a value whose size is determined by the size properties of the content object. <br>Returns: true if the `GridUnitType` property is `Auto`; otherwise, false. |
| `IsStar` | Gets a value that indicates whether the `GridLength` holds a value that is expressed as a weighted proportion of available space. <br>Returns: true if the `GridUnitType` property is `Star`; otherwise, false. |
| `Value` | Gets a `Double` that represents the value of the `GridLength`. <br>Returns: A `Double` that represents the value of the current instance. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double)` | Initializes a new instance of the `GridLength` structure using the specified absolute value in pixels. <br>`pixels`: The absolute count of pixels to establish as the value. |
| `Constructor(Double,GridUnitType)` | Initializes a new instance of the `GridLength` structure and specifies what kind of value it holds. <br>`value`: The initial value of this instance of `GridLength`. <br>`type`: The `GridUnitType` held by this instance of `GridLength`. |
| `Equals(Object)` | Determines whether the specified object is equal to the current `GridLength` instance. <br>`oCompare`: The object to compare with the current instance. <br>Returns: true if the specified object has the same value and `GridUnitType` as the current instance; otherwise, false. |
| `Equals(GridLength)` | Determines whether the specified `GridLength` is equal to the current `GridLength`. <br>`gridLength`: The `GridLength` structure to compare with the current instance. <br>Returns: true if the specified `GridLength` has the same value and `GridUnitType` as the current instance; otherwise, false. |
| `GetHashCode` | Gets a hash code for the `GridLength`. <br>Returns: A hash code for the `GridLength`. |
| `ToString` | Returns a `String` representation of the `GridLength`. <br>Returns: A `String` representation of the current `GridLength` structure. |
| `op_Equality(GridLength,GridLength)` | Compares two `GridLength` structures for equality. <br>`gl1`: The first instance of `GridLength` to compare. <br>`gl2`: The second instance of `GridLength` to compare. <br>Returns: true if the two instances of `GridLength` have the same value and `GridUnitType`; otherwise, false. |
| `op_Inequality(GridLength,GridLength)` | Compares two `GridLength` structures to determine if they are not equal. <br>`gl1`: The first instance of `GridLength` to compare. <br>`gl2`: The second instance of `GridLength` to compare. <br>Returns: true if the two instances of `GridLength` do not have the same value and `GridUnitType`; otherwise, false. |

## System.Windows.GridUnitType

Describes the kind of value that a `GridLength` object is holding.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Auto` | The size is determined by the size properties of the content object. |
| `Pixel` | The value is expressed in pixels. |
| `Star` | The value is expressed as a weighted proportion of available space. |

## System.Windows.HorizontalAlignment

Indicates where an element should be displayed on the horizontal axis relative to the allocated layout slot of the parent element.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Center` | An element aligned to the center of the layout slot for the parent element. |
| `Left` | An element aligned to the left of the layout slot for the parent element. |
| `Right` | An element aligned to the right of the layout slot for the parent element. |
| `Stretch` | An element stretched to fill the entire layout slot of the parent element. |

## System.Windows.IApplicationLifetimeAware

Defines methods that application extension services can optionally implement in order to respond to application lifetime events.

### Methods

| Member | Description |
|---|---|
| `Exited` | Called by an application immediately after the `Exit` event occurs. |
| `Exiting` | Called by an application immediately before the `Exit` event occurs. |
| `Started` | Called by an application immediately after the `Startup` event occurs. |
| `Starting` | Called by an application immediately before the `Startup` event occurs. |

## System.Windows.IApplicationService

Defines methods that application extension services must implement in order to enable an application to start and stop the service.

### Methods

| Member | Description |
|---|---|
| `StartService(ApplicationServiceContext)` | Called by an application in order to initialize the application extension service. <br>`context`: Provides information about the application state. |
| `StopService` | Called by an application in order to stop the application extension service. |

## System.Windows.IDataObject

Provides a format-independent mechanism for transferring data.

### Methods

| Member | Description |
|---|---|
| `GetData(String)` | Retrieves a data object in a specified format; the data format is specified by a string. <br>`format`: A string that specifies the format in which to retrieve the data. See the `DataFormats` class for a set of predefined data formats. <br>Returns: A data object that has the data in the specified format; or null, if the data is not available in the specified format. |
| `GetData(String,Boolean)` | Retrieves a data object in a specified format, and optionally, converts the data to the specified format. <br>`format`: A string that specifies the format in which to retrieve the data. See the `DataFormats` class for a set of predefined data formats. <br>`autoConvert`: true to attempt to automatically convert the data to the specified format; false to perform no data format conversion.If this parameter is false, the method returns data in the specified format if it is available; or returns null if the data is not available in the specified format. <br>Returns: A data object that has the data in the specified format; or null, if the data is not available in the specified format. |
| `GetData(Type)` | Retrieves a data object in a specified format; the data format is specified by a `Type` object. <br>`format`: A `Type` object that specifies the format in which to retrieve the data. See the `DataFormats` class for a set of predefined data formats. <br>Returns: A data object that has the data in the specified format; or null, if the data is not available in the specified format. |
| `GetDataPresent(String)` | Checks whether the data is available in, or can be converted to, a specified format; the data format is specified by a string. <br>`format`: A string that specifies the format to check for. See the `DataFormats` class for a set of predefined data formats. <br>Returns: true if the data is in, or can be converted to, the specified format; otherwise, false. |
| `GetDataPresent(String,Boolean)` | Checks whether the data is available in, or can be converted to, a specified format. If the data is not already available in the specified format, a Boolean flag indicates whether to check if the data can be converted to the specified format. <br>`format`: A string that specifies the format to check for. See the `DataFormats` class for a set of predefined data formats. <br>`autoConvert`: false to only check for the specified format; true to also check whether the data that is stored in this data object can be converted to the specified format. <br>Returns: true if the data is in, or can be converted to, the specified format; otherwise, false. |
| `GetDataPresent(Type)` | Checks to see whether the data is available in, or can be converted to, a specified format. The data format is specified by a `Type` object. <br>`format`: A `Type` that specifies the format to check for. <br>Returns: true if the data is in, or can be converted to, the specified format; otherwise, false. |
| `GetFormats` | Returns a list of all formats that the data in this data object is stored in, or can be converted to. <br>Returns: An array of strings, with each string specifying the name of a format that is supported by this data object. |
| `GetFormats(Boolean)` | Returns a list of all formats that the data in this data object is stored in. A Boolean flag indicates whether to also include formats that the data can be automatically converted to. <br>`autoConvert`: true to retrieve all formats that the data in this data object is stored in, or can be converted to; false to retrieve only the formats in which the data in this data object is stored (excludes formats that the data is not stored in, but can be automatically converted to). <br>Returns: An array of strings, with each string specifying the name of a format that is supported by this data object. |
| `SetData(Object)` | Stores the specified data in this data object, and automatically converts the data format from the source object type. <br>`data`: The data to store in this data object. |
| `SetData(String,Object)` | Stores the specified data in this data object, using one or more specified data formats. The data format is specified by a string. <br>`format`: A string that specifies the format in which to store the data. See the `DataFormats` class for a set of predefined data formats. <br>`data`: The data to store in this data object. |
| `SetData(String,Object,Boolean)` | Stores the specified data in this data object, using one or more specified data formats. This overload includes a Boolean flag to indicate whether the data may be converted to another format on retrieval. <br>`format`: A string that specifies the format in which to store the data. See the `DataFormats` class for a set of predefined data formats. <br>`data`: The data to store in this data object. <br>`autoConvert`: true to allow the data to be converted to another format on retrieval; false to prohibit the data from being converted to another format on retrieval. |
| `SetData(Type,Object)` | Stores the specified data in this data object, using one or more specified data formats. The data format is specified by a `Type` class. <br>`format`: A `Type` that specifies the format in which to store the data. <br>`data`: The data to store in this data object. |

## System.Windows.Icon

Represents an icon that is used to identify an offline application.

### Properties / fields

| Member | Description |
|---|---|
| `Size` | Gets the icon size. <br>Returns: The icon size. |
| `Source` | Gets the path and file name to the PNG source file of the icon. <br>Returns: The path to the PNG source file of the icon. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SizeProperty` | Identifies the `Size` dependency property. <br>Returns: The identifier for the `Size` dependency property. |
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Icon` class. |

## System.Windows.IconCollection

Represents a collection of `Icon` instances.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `IconCollection` class. |

## System.Windows.Ink.DrawingAttributes

Specifies drawing attributes that are used to draw a `Stroke`.

### Properties / fields

| Member | Description |
|---|---|
| `Color` | Gets or sets the color that is used to draw a `Stroke`. <br>Returns: The color that is used to draw a `Stroke`. The default is Black. |
| `Height` | Gets or sets the height of the stylus that is used to draw a `Stroke`. <br>Returns: The height of the stylus that is used to draw a `Stroke`. The default is 2 pixels. |
| `OutlineColor` | Gets or sets the outline color that is used to draw a `Stroke`. <br>Returns: The outline color of the stylus that is used to draw a `Stroke`. The default is Black. |
| `Width` | Gets or sets the width of the stylus that is used to draw a `Stroke`. <br>Returns: The width of the stylus that is used to draw a `Stroke`. The default is 2 pixels. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DrawingAttributes` class. |

## System.Windows.Ink.Stroke

Represents a collection of points that correspond to a stylus-down, move, and stylus-up sequence.

### Properties / fields

| Member | Description |
|---|---|
| `DrawingAttributes` | Gets or sets the properties of the stroke, such as `Height`, `Width`, `Color`, or `OutlineColor`. <br>Returns: The `DrawingAttributes` of the stroke. |
| `StylusPoints` | Gets or sets the stylus points of the `Stroke`. <br>Returns: The `StylusPointCollection` that contains the stylus points that represent the current `Stroke`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Stroke` class. |
| `Constructor(Input.StylusPointCollection)` | Initializes a new instance of the `Stroke` class with the specified `StylusPointCollection`. <br>`stylusPoints`: A `StylusPointCollection` that represents the `Stroke`. |
| `GetBounds` | Retrieves the bounding box for the `Stroke` object. <br>Returns: A `Rect` structure defining the bounding box for the `Stroke` object. |
| `HitTest(Input.StylusPointCollection)` | Indicates whether a specified `StylusPointCollection` intersects with a `Stroke` object. <br>`stylusPointCollection`: The `StylusPointCollection` used to check for intersection with the `Stroke` object. <br>Returns: true if the specified `StylusPointCollection` intersects with the `Stroke` object; otherwise, false. |

## System.Windows.Ink.StrokeCollection

Represents a collection of `Stroke` objects.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `StrokeCollection` class. |
| `GetBounds` | Returns the bounds of the strokes in the collection. <br>Returns: A `Rect` that contains the bounds of the strokes in the `StrokeCollection`. |
| `HitTest(Input.StylusPointCollection)` | Indicates whether a specified `StylusPointCollection` intersects with a `StrokeCollection` object. <br>`stylusPointCollection`: The `StylusPointCollection` used to check for intersection with the `Stroke` object. <br>Returns: A `StrokeCollection` that contains the strokes that intersect with the points in the specified `StylusPointCollection`. |

## System.Windows.Input.Cursor

Represents the image used for the mouse pointer.

### Methods

| Member | Description |
|---|---|
| `ToString` | Returns the string representation of the `Cursor`. <br>Returns: The string representation of the cursor. This corresponds to the active `Cursors` property name. |

## System.Windows.Input.Cursors

Defines a set of default mouse pointer images for Silverlight.

### Properties / fields

| Member | Description |
|---|---|
| `Arrow` | Represents an Arrow `Cursor`. <br>Returns: An Arrow `Cursor`. |
| `Eraser` | Represents an Eraser `Cursor`. <br>Returns: An Eraser `Cursor`. |
| `Hand` | Represents a Hand `Cursor`. <br>Returns: A Hand `Cursor`. |
| `IBeam` | Represents an IBeam `Cursor`, which is typically used to show where the text cursor appears when the mouse is clicked. <br>Returns: An IBeam `Cursor`. |
| `None` | Represents a special `Cursor` that is invisible. <br>Returns: The None `Cursor`. |
| `SizeNESW` | Represents a SizeNESW `Cursor`. <br>Returns: A SizeNESW `Cursor`. |
| `SizeNS` | Represents a SizeNS `Cursor`. <br>Returns: A SizeNS `Cursor`. |
| `SizeNWSE` | Represents a SizeNWSE `Cursor`. <br>Returns: A SizeNWSE `Cursor`. |
| `SizeWE` | Represents a SizeWE `Cursor`. <br>Returns: A SizeWE `Cursor`. |
| `Stylus` | Represents a Stylus `Cursor`. <br>Returns: A Stylus `Cursor`. |
| `Wait` | Represents a Wait`Cursor`. <br>Returns: A Wait `Cursor`. |

## System.Windows.Input.FocusManager

Provides utility methods related to element focus, without the need to handle focus-related events.

### Methods

| Member | Description |
|---|---|
| `GetFocusedElement` | Queries the Silverlight focus system to determine which object has focus. <br>Returns: The object that currently has focus. Typically, this is a `Control` class. |
| `GetFocusedElement(DependencyObject)` | Gets the element with focus within the specified focus scope. <br>`element`: Declares the scope. <br>Returns: The element in the specified focus scope that has current focus. |

## System.Windows.Input.GestureEventArgs

Provides event data for gesture events such as `DoubleTap`.

### Properties / fields

| Member | Description |
|---|---|
| `Handled` | Gets or sets a value that marks the routed event as handled. A true value for `Handled` prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled; false to leave the routed event unhandled, which permits the event to potentially route further. The default is false. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GestureEventArgs` class. |
| `GetPosition(UIElement)` | Returns the x- and y-coordinates of the pointer position, optionally evaluated against a coordinate origin of a supplied T:System.Windows.UIElement . <br>`relativeTo`: Any `UIElement`-derived object that is contained by the Silverlight plug-in and connected to the object tree. To specify the object relative to the overall Silverlight coordinate system, pass null. <br>Returns: A `Point` that represents the current x- and y-coordinates of the mouse pointer position. If null was passed as `relativeTo`, this coordinate is for the overall Silverlight plug-in content area. If a non-null `relativeTo` was passed, this coordinate is relative to the object referenced by `relativeTo`. |

## System.Windows.Input.ICommand

Defines the contract for commanding.

### Events

| Member | Description |
|---|---|
| `CanExecuteChanged` | Occurs when changes occur that affect whether the command should execute. |

### Methods

| Member | Description |
|---|---|
| `CanExecute(Object)` | Defines the method that determines whether the command can execute in its current state. <br>`parameter`: Data used by the command. If the command does not require data to be passed, this object can be set to null. <br>Returns: true if this command can be executed; otherwise, false. |
| `Execute(Object)` | Defines the method to be called when the command is invoked. <br>`parameter`: Data used by the command. If the command does not require data to be passed, this object can be set to null. |

## System.Windows.Input.ImeConversionModeValues

Describes a mode of input conversion to be performed by an input method editor interacting with a Silverlight-based application.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Alphanumeric` | The input method uses alphanumeric conversion mode. |
| `CharCode` | The input method uses character code conversion mode. |
| `DoNotCare` | The input method does not care what input conversion method is used; the actual conversion method is indeterminate. |
| `Eudc` | The input method uses EUDC (end user defined character) conversion mode. |
| `Fixed` | The input method uses fixed conversion mode. |
| `FullShape` | The input method uses full-shape conversion mode. |
| `Zatakana` | The input method uses Zatakana conversion mode. |
| `Native` | The input method uses a native character (Hiragana, Hangul, Chinese) conversion mode. |
| `NoConversion` | The input method will not perform any input conversion. |
| `Roman` | The input method uses Roman character conversion mode. |
| `Symbol` | The input method uses symbol conversion mode. |

## System.Windows.Input.InputMethod

Specifies attached properties that influence input method editor (IME) features and support when used with a Silverlight-based application.

### Properties / fields

| Member | Description |
|---|---|
| `IsInputMethodEnabled` | Gets or sets a value that determines whether input method editors can be used to provide input for the control where the property is attached. <br>Returns: true if input method editors can be used to provide input; otherwise, false. |
| `PreferredImeConversionMode` | Gets or sets a preferred `ImeConversionModeValues` value for a specified dependency object. <br>Returns: A preferred `ImeConversionModeValues` value for a specified dependency object. |
| `PreferredImeState` | Gets or sets a preferred input method state for a specified dependency object. <br>Returns: A preferred input method state for a specified dependency object. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsInputMethodEnabledProperty` | Identifies the `IsInputMethodEnabled` attached property. <br>Returns: The identifier for the `IsInputMethodEnabled` attached property. |
| `PreferredImeConversionModeProperty` | Identifies the `PreferredImeConversionMode` attached property. <br>Returns: The identifier for the `PreferredImeConversionMode` attached property. |
| `PreferredImeStateProperty` | Identifies the `PreferredImeState` attached property. <br>Returns: The identifier for the `PreferredImeState` attached property. |

### Methods

| Member | Description |
|---|---|
| `GetIsInputMethodEnabled(DependencyObject)` | Gets the value for the `IsInputMethodEnabled` attached property on the specified object. <br>`target`: The object to get the `IsInputMethodEnabled` attached property value from. <br>Returns: The `IsInputMethodEnabled` attached property value. |
| `GetPreferredImeConversionMode(DependencyObject)` | Gets the value for the `PreferredImeConversionMode` attached property on the specified object. <br>`target`: The object to get the `PreferredImeConversionMode` attached property value from. <br>Returns: The `PreferredImeConversionMode` attached property value. |
| `GetPreferredImeState(DependencyObject)` | Gets the value for the `PreferredImeState` attached property on the specified object. <br>`target`: The object to get the `PreferredImeState` attached property value from. <br>Returns: The `PreferredImeState` attached property value. |
| `SetIsInputMethodEnabled(DependencyObject,Boolean)` | Sets the value for the `IsInputMethodEnabled` attached property on the specified object. <br>`target`: The object on which to set the `IsInputMethodEnabled` attached property value. <br>`value`: The value to set. |
| `SetPreferredImeConversionMode(DependencyObject,Input.ImeConversionModeValues)` | Sets the value for the `PreferredImeConversionMode` attached property on the specified object. <br>`target`: The object on which to set the `PreferredImeConversionMode` attached property value. <br>`value`: The value to set. |
| `SetPreferredImeState(DependencyObject,Input.InputMethodState)` | Sets the value for the `PreferredImeState` attached property on the specified object. <br>`target`: The object on which to set the `PreferredImeState` attached property value. <br>`value`: The value to set. |

## System.Windows.Input.InputMethodState

Describes the state of an input method editor when it interacts with a Silverlight-based application.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DoNotCare` | The input method is in an indeterminate state. |
| `Off` | The input method editor is off for purposes of generating device independent input. |
| `On` | The input method editor is on. |

## System.Windows.Input.InputScope

Represents information related to the scope of data provided by an input method.

### Properties / fields

| Member | Description |
|---|---|
| `Names` | Gets or sets the input scope names, as a list of strings. <br>Returns: The input scope names, as a list of strings. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `InputScope` class. |

## System.Windows.Input.InputScopeConverter

Provides type conversion support for the `InputScope` class.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `InputScopeConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Returns whether this converter can convert an object of one type to the `InputScope` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: A `Type` that represents the type you want to convert from. <br>Returns: true if `sourceType` is type `String`; otherwise, false. |
| `CanConvertTo(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether conversion is possible to the specified type from a `InputScope` object. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`destinationType`: The intended destination type for the conversion. <br>Returns: This implementation always returns false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to an `InputScope` object. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: Describes the intended `CultureInfo` for the conversion. <br>`source`: The object that the converter should convert. <br>Returns: The `InputScope` created from converting source. |
| `ConvertTo(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object,Type)` | Converts the specified value object to the specified type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: Describes the intended `CultureInfo` for the conversion. <br>`value`: The object to convert. <br>`destinationType`: The intended destination type for the conversion. <br>Returns: The converted object. |

## System.Windows.Input.InputScopeName

Defines a name for text input patterns.

### Properties / fields

| Member | Description |
|---|---|
| `NameValue` | Gets or sets the input scope name value, which modifies how input from alternative input methods is interpreted. <br>Returns: A value of the enumeration. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `InputScopeName` class. |
| `Constructor(Input.InputScopeNameValue)` | Initializes a new instance of the `InputScopeName` class using an initializing `InputScopeNameValue`. <br>`nameValue`: The input scope name value, as a value of the `InputScopeNameValue` enumeration. |

## System.Windows.Input.InputScopeNameConverter

Provides type conversion support for the `InputScopeName` class.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `InputScopeNameConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Returns whether this converter can convert an object of one type to the `InputScopeName` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: A `Type` that represents the type you want to convert from. <br>Returns: true if `sourceType` is type `String`; otherwise, false. |
| `CanConvertTo(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether conversion is possible to the specified type from a `InputScopeName` object. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`destinationType`: The intended destination type for the conversion. <br>Returns: true if `destinationType` is a type that this converter can convert to `InputScopeName`; otherwise, false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to an `InputScopeName` object. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: Describes the intended `CultureInfo` for the conversion. <br>`source`: The object that the converter should convert. <br>Returns: The `InputScopeName` created from converting source. |
| `ConvertTo(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object,Type)` | Converts the specified value object to the specified type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: Describes the intended `CultureInfo` for the conversion. <br>`value`: The object to convert. <br>`destinationType`: The intended destination type for the conversion. <br>Returns: The converted object. |

## System.Windows.Input.InputScopeNameValue

Specifies the input scope name, which modifies how input from alternative input methods is interpreted.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AddressCity` | The text input pattern for a city address. |
| `AddressCountryName` | The text input pattern for the name of a country/region. |
| `AddressCountryShortName` | The text input pattern for the abbreviated name of a country/region. |
| `AddressStateOrProvince` | The text input pattern for a state or province. |
| `AddressStreet` | The text input pattern for a street address. |
| `AlphanumericFullWidth` | The text input pattern for alphanumeric full-width characters. |
| `AlphanumericHalfWidth` | The text input pattern for alphanumeric half-width characters. |
| `ApplicationEnd` | Not supported. For internal use in Silverlight for Windows Phone. |
| `Bopomofo` | The text input pattern for the Bopomofo Mandarin Chinese phonetic transcription system. |
| `Chat` | The SIP layout for text messaging, which recognizes pre-defined abbreviations. Supported only in Silverlight for Windows Phone. |
| `CurrencyAmount` | The text input pattern for amount of currency. |
| `CurrencyAmountAndSymbol` | The text input pattern for amount and symbol of currency. |
| `CurrencyChinese` | The text input pattern for Chinese currency. |
| `Date` | The text input pattern for a calendar date. |
| `DateDay` | The text input pattern for the numeric day in a calendar date. |
| `DateDayName` | The text input pattern for the name of the day in a calendar date. |
| `DateMonth` | The text input pattern for the numeric month in a calendar date. |
| `DateMonthName` | The text input pattern for the name of the month in a calendar date. |
| `DateYear` | The text input pattern for the year in a calendar date. |
| `Default` | The default handling of input commands. |
| `Digits` | The text input pattern for digits. |
| `EmailNameOrAddress` | The SIP layout for an e-mail name or address. Supported only in Silverlight for Windows Phone. |
| `EmailSmtpAddress` | The text input pattern for a Simple Mail Transfer Protocol (SMTP) email address. |
| `EmailUserName` | The text input pattern for an email user name. |
| `EnumString` | Not supported. For internal use in Silverlight for Windows Phone. |
| `FileName` | The text input pattern for a file name. |
| `Formula` | The text input pattern for a formula to be evaluated. |
| `FullFilePath` | The text input pattern for the full path of a file. |
| `Hanja` | The text input pattern for Hanja characters. |
| `Hiragana` | The text input pattern for the Hiragana writing system. |
| `ZatakanaFullWidth` | The text input pattern for full-width Zatakana characters. |
| `ZatakanaHalfWidth` | The text input pattern for half-width Zatakana characters. |
| `LogOnName` | The text input pattern for a log on name. |
| `Maps` | The SIP layout for entering a map location. Supported only in Silverlight for Windows Phone. |
| `NameOrPhoneNumber` | The SIP layout for SMS To field. Supported only in Silverlight for Windows Phone. |
| `Number` | The text input pattern for a number. |
| `NumberFullWidth` | The text input pattern for a full-width number. |
| `NumericPassword` | The text input pattern for a numeric password. |
| `OneChar` | The text input pattern for one character. |
| `Password` | The text input pattern for a password. |
| `PersonalFullName` | The text input pattern for a person's full name. |
| `PersonalGivenName` | The text input pattern for a person's given name. |
| `PersonalMiddleName` | The text input pattern for a person's middle name. |
| `PersonalNamePrefix` | The text input pattern for the prefix of a person's name. |
| `PersonalNameSuffix` | The text input pattern for the suffix of a person's name. |
| `PersonalSurname` | The text input pattern for a person's surname. |
| `PhraseList` | The text input pattern for a phrase list. |
| `PostalAddress` | The text input pattern for a postal address. |
| `PostalCode` | The text input pattern for a postal code. |
| `Private` | Not supported. For internal use in Silverlight for Windows Phone. |
| `RegularExpression` | The text input pattern for a regular expression. |
| `Search` | The SIP layout for a search query. Supported only in Silverlight for Windows Phone. |
| `Srgs` | The text input pattern for the Speech Recognition Grammar Specification (SRGS). |
| `TelephoneAreaCode` | The text input pattern for a telephone area code. |
| `TelephoneCountryCode` | The text input pattern for a telephone country/region code. |
| `TelephoneLocalNumber` | The text input pattern for a telephone local number. |
| `TelephoneNumber` | The text input pattern for a telephone number. |
| `Text` | The software input panel (SIP) layout for standard text input. Supported only in Silverlight for Windows Phone. |
| `Time` | The text input pattern for the time. |
| `TimeHour` | The text input pattern for the hour of the time. |
| `TimeMinorSec` | The text input pattern for the minutes or seconds of time. |
| `Url` | The text input pattern for a Uniform Resource Locator (URL). |
| `Xml` | The text input pattern for XML. |
| `Yomi` | Not supported. For internal use in Silverlight for Windows Phone. |

## System.Windows.Input.Zey

Specifies the possible key values on a keyboard.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `A` | The A key. |
| `Add` | The + (ADD) key. |
| `Alt` | The ALT key. |
| `B` | The B key. |
| `Back` | The BACKSPACE key. |
| `C` | The C key. |
| `CapsLock` | The CAPSLOCK key. |
| `Ctrl` | The CTRL (control) key. |
| `D` | The D key. |
| `D0` | The 0 (zero) key. |
| `D1` | The 1 key. |
| `D2` | The 2 key. |
| `D3` | The 3 key. |
| `D4` | The 4 key. |
| `D5` | The 5 key. |
| `D6` | The 6 key. |
| `D7` | The 7 key. |
| `D8` | The 8 key. |
| `D9` | The 9 key. |
| `Decimal` | The . (DECIMAL) key. |
| `Delete` | The DEL (also known as DELETE) key. |
| `Divide` | The / (DIVIDE) key. |
| `Down` | The down arrow key. |
| `E` | The E key. |
| `End` | The END key. |
| `Enter` | The ENTER key. |
| `Escape` | The ESC (also known as ESCAPE) key. |
| `F` | The F key. |
| `F1` | The F1 key. |
| `F10` | The F10 key. |
| `F11` | The F11 key. |
| `F12` | The F12 key. |
| `F2` | The F2 key. |
| `F3` | The F3 key. |
| `F4` | The F4 key. |
| `F5` | The F5 key. |
| `F6` | The F6 key. |
| `F7` | The F7 key. |
| `F8` | The F8 key. |
| `F9` | The F9 key. |
| `G` | The G key. |
| `H` | The H key. |
| `Home` | The HOME key. |
| `I` | The I key. |
| `Insert` | The INSERT key. |
| `J` | The J key. |
| `K` | The K key. |
| `L` | The L key. |
| `Left` | The left arrow key. |
| `M` | The M key. |
| `Multiply` | The * (MULTIPLY) key. |
| `N` | The N key. |
| `None` | A special value indicating no key. |
| `NumPad0` | The 0 key on the number pad. |
| `NumPad1` | The 1 key on the number pad. |
| `NumPad2` | The 2 key on the number pad. |
| `NumPad3` | The 3 key on the number pad. |
| `NumPad4` | The 4 key on the number pad. |
| `NumPad5` | The 5 key on the number pad. |
| `NumPad6` | The 6 key on the number pad. |
| `NumPad7` | The 7 key on the number pad. |
| `NumPad8` | The 8 key on the number pad. |
| `NumPad9` | The 9 key on the number pad. |
| `O` | The O key. |
| `P` | The P key. |
| `PageDown` | The PAGEDOWN key. |
| `PageUp` | The PAGEUP key. |
| `Q` | The Q key. |
| `R` | The R key. |
| `Right` | The right arrow key. |
| `S` | The S key. |
| `Shift` | The SHIFT key. |
| `Space` | The SPACE key. |
| `Subtract` | The - (SUBTRACT) key. |
| `T` | The T key. |
| `Tab` | The TAB key. |
| `U` | The U key. |
| `Unknown` | A special value indicating the key is out of range of this enumeration. |
| `Up` | The up arrow key. |
| `V` | The V key. |
| `W` | The W key. |
| `X` | The X key. |
| `Y` | The Y key. |
| `Z` | The Z key. |

## System.Windows.Input.ZeyEventArgs

Provides event data for the `ZeyUp` and `ZeyDown` events.

### Properties / fields

| Member | Description |
|---|---|
| `Handled` | Gets or sets a value that marks the routed event as handled. A true value for Handled prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled; false to leave the routed event unhandled, which permits the event to potentially route further. The default is false. |
| `Zey` | Gets the keyboard key associated with the event. <br>Returns: One of the enumeration values that indicates the key referenced by the event. |
| `PlatformKeyCode` | Gets an integer value that represents the key that is pressed or released (depending on which event is raised). This value is the nonportable key code, which is operating system–specific. <br>Returns: The key code value. |

## System.Windows.Input.ZeyEventHandler

Represents the method that handles the `ZeyUp` and `ZeyDown` events.

## System.Windows.Input.Zeyboard

Represents the keyboard device.

### Properties / fields

| Member | Description |
|---|---|
| `Modifiers` | Gets the set of `ModifierKeys` that are currently pressed. <br>Returns: A bitwise combination of the enumeration values. |

## System.Windows.Input.ZeyboardNavigationMode

Specifies the tabbing behavior across tab stops for a Silverlight tabbing sequence within a container.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Cycle` | Focus returns to the first or the last keyboard navigation stop inside of a container when the first or last keyboard navigation stop is reached. |
| `Local` | Tab indexes are considered on the local subtree only inside this container. |
| `Once` | The container and all of its child elements as a whole receive focus only once. |

## System.Windows.Input.ManipulationCompletedEventArgs

Provides data for the `ManipulationCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `FinalVelocities` | Gets the velocities that are used for the manipulation. <br>Returns: The velocities that are used for the manipulation. |
| `Handled` | Gets or sets a value that marks the routed event as handled. Setting to true prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled; false to leave the routed event unhandled, which permits the event to potentially route further. The default is false. |
| `IsInertial` | Gets whether the `ManipulationCompleted` event occurs during inertia. <br>Returns: true if the `ManipulationCompleted` event occurs during inertia; false if the event occurs while the user's input device has contact with the element. |
| `ManipulationContainer` | Gets the container that defines the coordinates for the manipulation. <br>Returns: The container element. |
| `ManipulationOrigin` | Gets the point from which the manipulation originated. <br>Returns: The point from which the manipulation originated. |
| `TotalManipulation` | Gets the total transformation that occurs during the current manipulation. <br>Returns: The total transformation that occurs during the current manipulation. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ManipulationCompletedEventArgs` class. |

## System.Windows.Input.ManipulationDelta

Contains transformation data that is accumulated when manipulation events occur.

### Properties / fields

| Member | Description |
|---|---|
| `Scale` | Gets the amount the manipulation has resized as a multiplier. <br>Returns: The amount the manipulation has resized as a multiplier. |
| `Translation` | Gets the linear motion of the manipulation. <br>Returns: The linear motion of the manipulation. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ScaleProperty` | Identifies the `Scale` dependency property. <br>Returns: The identifier for the `Scale` dependency property. |
| `TranslationProperty` | Identifies the `Translation` dependency property. <br>Returns: The identifier for the `Translation` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Point,Point)` | Initializes a new instance of the `ManipulationDelta` class, using initial data. <br>`translation`: Translation transformation to report. <br>`scale`: Initial scale transformation to report. |

## System.Windows.Input.ManipulationDeltaEventArgs

Provides data for the `ManipulationDelta` event.

### Properties / fields

| Member | Description |
|---|---|
| `CumulativeManipulation` | Gets the accumulated changes of the current manipulation, as a `ManipulationDelta`. <br>Returns: The accumulated changes of the current manipulation. |
| `DeltaManipulation` | Gets the most recent changes of the current manipulation, as a `ManipulationDelta`. <br>Returns: The most recent changes of the current manipulation. |
| `Handled` | Gets or sets a value that marks the routed event as handled. Setting to true prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled; false to leave the routed event unhandled, which permits the event to potentially route further. The default is false. |
| `IsInertial` | Gets whether the `ManipulationDelta` event occurs during inertia. <br>Returns: true if the `ManipulationDelta` event occurs during inertia; false if the event occurs while the user's input device has contact with the element. |
| `ManipulationContainer` | Gets the container that defines the coordinates for the manipulation. <br>Returns: The container element. |
| `ManipulationOrigin` | Gets the point from which the manipulation originated. <br>Returns: The point from which the manipulation originated. |
| `Velocities` | Gets the rates of the most recent changes to the manipulation. <br>Returns: The rates of the most recent changes to the manipulation. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ManipulationDeltaEventArgs` class. |
| `Complete` | Completes the manipulation without inertia. |
| `StartInertia` | Starts inertia on the manipulation by ignoring subsequent contact movements. |

## System.Windows.Input.ManipulationStartedEventArgs

Provides data for the `ManipulationStarted` event.

### Properties / fields

| Member | Description |
|---|---|
| `Handled` | Gets or sets a value that marks the routed event as handled. Setting to true prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled; false to leave the routed event unhandled, which permits the event to potentially route further. The default is false. |
| `ManipulationContainer` | Gets the container that defines the coordinates for the manipulation. <br>Returns: The container element. |
| `ManipulationOrigin` | Gets the point from which the manipulation originated. <br>Returns: The point from which the manipulation originated. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ManipulationStartedEventArgs` class. |
| `Complete` | Completes the manipulation without inertia. |

## System.Windows.Input.ManipulationVelocities

Describes the speed at which manipulations occurs.

### Properties / fields

| Member | Description |
|---|---|
| `ExpansionVelocity` | Gets the rate at which the manipulation resized. <br>Returns: The rate at which the manipulation resized. |
| `LinearVelocity` | Gets the speed of linear motion. <br>Returns: The speed of linear motion. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ExpansionVelocityProperty` | Identifies the `ExpansionVelocity` dependency property. <br>Returns: The identifier for the `ExpansionVelocity` dependency property. |
| `LinearVelocityProperty` | Identifies the `LinearVelocity` dependency property. <br>Returns: The identifier for the `LinearVelocity` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Point,Point)` | Initializes a new instance of the `ManipulationVelocities` class using initial values. <br>`linear`: Linear rate to report. <br>`expansion`: Expansion rate to report. |

## System.Windows.Input.ModifierKeys

Specifies the set of modifier keys.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Alt` | The ALT key is pressed. |
| `Apple` | The Apple key (also known as the Open Apple key) is pressed. |
| `Control` | The CTRL key is pressed. |
| `None` | No modifiers are pressed. |
| `Shift` | The SHIFT key is pressed. |
| `Windows` | The Windows logo key is pressed. |

## System.Windows.Input.MouseButtonEventArgs

Provides event data for mouse button input events, for example `MouseLeftButtonDown` and `MouseRightButtonUp`.

### Properties / fields

| Member | Description |
|---|---|
| `ClickCount` | Gets the number of times the button was clicked. <br>Returns: The number of times the mouse button was clicked. |
| `Handled` | Gets or sets a value that marks the routed event as handled. A true value for Handled prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled. false to leave the routed event unhandled, which permits the event to potentially route further and be acted on by other handlers. The default is false. |

## System.Windows.Input.MouseButtonEventHandler

Represents the method that handles the `MouseLeftButtonDown` and `MouseLeftButtonUp` events.

## System.Windows.Input.MouseEventArgs

Provides data for mouse-related events that do not specifically involve mouse buttons, for example `MouseMove`.

### Properties / fields

| Member | Description |
|---|---|
| `StylusDevice` | Gets an object that reports stylus device information, such as the collection of stylus points associated with the input. <br>Returns: The stylus device information object. |

### Methods

| Member | Description |
|---|---|
| `GetPosition(UIElement)` | Returns the x- and y-coordinates of the mouse pointer position, optionally evaluated against a coordinate origin of a supplied `UIElement`. <br>`relativeTo`: Any `UIElement`-derived object that is contained by the Silverlight plug-in and connected to the object tree. To specify the object relative to the overall Silverlight coordinate system, use a `relativeTo` value of null. <br>Returns: A `Point` that represents the current x- and y-coordinates of the mouse pointer position. If null was passed as `relativeTo`, this coordinate is for the overall Silverlight plug-in content area. If a non-null `relativeTo` was passed, this coordinate is relative to the object referenced by `relativeTo`. |

## System.Windows.Input.MouseEventHandler

Represents the method that will handle mouse related routed events that do not specifically involve mouse buttons; for example, `MouseMove`.

## System.Windows.Input.MouseWheelEventArgs

Provides data for the `MouseWheel` routed event.

### Properties / fields

| Member | Description |
|---|---|
| `Delta` | Gets a value that indicates the amount that the mouse wheel rotated relative to its starting state or to the last occurrence of the event. <br>Returns: An integer value that provides a factor of how much the mouse wheel rotated. This value can be a negative integer; see Remarks. |
| `Handled` | Gets or sets a value that marks the routed event as handled, and prevents most handlers along the event route from rehandling the same event. <br>Returns: true if the routed event is marked as handled. false if the routed event is kept unhandled, which permits the event to potentially route further and be acted on by other handlers. The default is false. |

## System.Windows.Input.MouseWheelEventHandler

Represents the method that will handle the `MouseWheel` routed event.

## System.Windows.Input.StylusDevice

Reports stylus device information, such as the collection of stylus points associated with the input.

### Properties / fields

| Member | Description |
|---|---|
| `DeviceType` | Gets the type of the tablet device. <br>Returns: The type of the tablet device. |
| `Inverted` | Gets or sets a value that indicates whether the stylus is inverted. <br>Returns: true if the stylus is inverted; otherwise, false. The default is false. |

### Methods

| Member | Description |
|---|---|
| `GetStylusPoints(UIElement)` | Returns the stylus points collected since the last mouse event. <br>`relativeTo`: Specifies the offset for the object (typically an `InkPresenter`) that should be applied to captured points. <br>Returns: A collection of the stylus points collected since the last mouse event. |

## System.Windows.Input.StylusPoint

Represents a single point collected while the user is entering ink strokes with the stylus or mouse.

### Properties / fields

| Member | Description |
|---|---|
| `PressureFactor` | Gets or sets the pressure factor of the stylus on the screen. <br>Returns: The pressure factor of the stylus on the screen. The default is 0.5. |
| `X` | Gets or sets the value for the x-coordinate of the `StylusPoint`. <br>Returns: The x-coordinate of the `StylusPoint` in a pixel grid. The default is 0. |
| `Y` | Gets or sets the value for the y-coordinate of the `StylusPoint`. <br>Returns: The y-coordinate of the `StylusPoint` in a pixel grid. The default is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double)` | Initializes a new instance of the `StylusPoint` class. <br>`x`: The x-coordinate of the `StylusPoint` in a pixel grid. <br>`y`: The y-coordinate of the `StylusPoint` in a pixel grid. |

## System.Windows.Input.StylusPointCollection

Represents a collection of related `StylusPoint` objects.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `StylusPointCollection` class. |
| `Add(Input.StylusPointCollection)` | Adds a collection of `StylusPoint` objects to the collection. <br>`stylusPoints`: The collection of `StylusPoint` objects to add to the collection. |

## System.Windows.Input.TabletDeviceType

Defines values for the type of devices the tablet device uses.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Mouse` | Indicates the tablet device is a mouse. |
| `Stylus` | Indicates the tablet device is a stylus. |
| `Touch` | Indicates the tablet device is a touch screen. |

## System.Windows.Input.TextComposition

Represents a composition related to text input which includes the composition text itself.

### Properties / fields

| Member | Description |
|---|---|
| `CompositionText` | Gets the composition text for this text composition. <br>Returns: The composition text for this text composition. |

## System.Windows.Input.TextCompositionEventArgs

Provides data for the `TextInput` routed event.

### Properties / fields

| Member | Description |
|---|---|
| `Handled` | Gets or sets a value that marks the routed event as handled, and prevents most handlers along the event route from handling the same event again. <br>Returns: true to mark the routed event handled. false to leave the routed event unhandled, which permits the event to potentially route further and be acted on by other handlers. The default is false. |
| `Text` | Gets or sets the text string that of the text composition. <br>Returns: The text string of the text composition. |
| `TextComposition` | Gets or sets the text in the composition as a `TextComposition` object. <br>Returns: The text in the composition, as a `TextComposition` object. |

## System.Windows.Input.TextCompositionEventHandler

Represents the method that will handle the `TextInput` routed event.

## System.Windows.Input.Touch

Provides an application-level service that processes touch input from the operating system and raises the Silverlight-specific `FrameReported` event.

### Events

| Member | Description |
|---|---|
| `FrameReported` | Occurs when the input system promotes a Windows 7 touch message for Silverlight. |

## System.Windows.Input.TouchAction

Describes the action of a specific touch point.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Down` | A touch action from a TOUCHEVENTF_DOWN message. |
| `Move` | A touch action from a TOUCHEVENTF_MOVE message. |
| `Up` | A touch action from a TOUCHEVENTF_UP message. |

## System.Windows.Input.TouchDevice

Identifies a "device" that produced the touch point. This is not a literal device, instead it distinguishes touch points through a down/move/up sequence.

### Properties / fields

| Member | Description |
|---|---|
| `DirectlyOver` | Gets the topmost `UIElement` object that the mouse is over at the time that this touch point was produced. <br>Returns: The topmost (in Z-order) `UIElement` object that the mouse is over at the time that this touch point was produced. |
| `Id` | Gets a unique ID as provided by the operating system for a particular touch device. <br>Returns: A unique ID for a particular touch device. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DirectlyOverProperty` | Identifies the `DirectlyOver` dependency property. <br>Returns: The identifier for the `DirectlyOver` dependency property. |
| `IdProperty` | Identifies the `Id` dependency property. <br>Returns: The identifier for the `Id` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TouchDevice` class. |

## System.Windows.Input.TouchFrameEventArgs

Provides data for the `FrameReported` event.

### Properties / fields

| Member | Description |
|---|---|
| `Timestamp` | Gets an integer time-stamp value for the event. <br>Returns: An integer time-stamp value for the event. |

### Methods

| Member | Description |
|---|---|
| `GetPrimaryTouchPoint(UIElement)` | Returns the primary touch point for the reported frame as a `TouchPoint` value. The touch point can be evaluated against a coordinate reference point instead of the absolute Silverlight coordinates. <br>`relativeTo`: A coordinate reference point that can be used to offset the coordinates of the return value `TouchPoint` . If you want the reference to be based on Silverlight absolute coordinates, specify `relativeTo` as null. <br>Returns: The primary touch point for the reported frame. |
| `GetTouchPoints(UIElement)` | Returns a collection of all `TouchPoint` values in the reported frame. The touch point can be evaluated against a coordinate reference point instead of the absolute Silverlight coordinates. <br>`relativeTo`: A coordinate reference point that can be used to offset the coordinates of all `TouchPoint` items in the returned collection. If you want the reference to be based on Silverlight absolute coordinates, specify `relativeTo` as null. <br>Returns: A collection of `TouchPoint` values. |
| `SuspendMousePromotionUntilTouchUp` | Disables automatic mouse-event promotion for the primary touch point until all touch points report as `Up`. |

## System.Windows.Input.TouchFrameEventHandler

Represents the method that will handle the `FrameReported` event of `Touch`.

## System.Windows.Input.TouchPoint

Represents a single touch point from a touch message source.

### Properties / fields

| Member | Description |
|---|---|
| `Action` | Gets the type of action (up, down, move) that the `TouchPoint` represents. <br>Returns: A value of the `TouchAction` enumeration. |
| `Position` | Gets the X,Y coordinate position of the touch point as a `Point`. <br>Returns: A `Point` value that reports the coordinate position of the touch point. |
| `Size` | Gets the rectangular area that is reported as the touch-point contact area. <br>Returns: A `Size` value. If the device that is used for touch input does not support reporting a touch size area, this property reports a `Size` with values of -1, instead of null. |
| `TouchDevice` | Gets the specific device type that produced the touch point. <br>Returns: A `TouchDevice` value that reports specific information about the generating device. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ActionProperty` | Identifies the `Action` dependency property. <br>Returns: The identifier for the `Action` dependency property. |
| `PositionProperty` | Identifies the `Position` dependency property. <br>Returns: The identifier for the `Position` dependency property. |
| `SizeProperty` | Identifies the `Size` dependency property. <br>Returns: The identifier for the `Size` dependency property. |
| `TouchDeviceProperty` | Identifies the `TouchDevice` dependency property. <br>Returns: The identifier for the `TouchDevice` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TouchPoint` class. |

## System.Windows.Input.TouchPointCollection

Contains a collection of `TouchPoint` values.

## System.Windows.InstallState

Defines constants that indicate the installation state of an application that is configured to run outside the browser.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `InstallFailed` | The application could not be installed to run outside the browser. |
| `Installed` | The application has been installed to run outside the browser. |
| `Installing` | The application is in the process of being installed to run outside the browser. |
| `NotInstalled` | The application has not been installed to run outside the browser. |

## System.Windows.Interop.Content

Reports details about the instantiation settings of a-based application, and exposes some of the HTML DOM values for the hosted Silverlight plug-in instance. The `Content` object contains APIs that specifically relate to the Silverlight content area (the area that is declared by the width and height in the initialization).

### Properties / fields

| Member | Description |
|---|---|
| `ActualHeight` | Gets the browser-determined height of the Silverlight plug-in content area. <br>Returns: The browser-determined height, in pixels, of the Silverlight plug-in content area. The default value is the height, in pixels, of the Silverlight plug-in, as specified by the HTML object element that instantiated it. |
| `ActualWidth` | Gets the browser-determined width of the Silverlight plug-in content area. <br>Returns: The browser-determined width, in pixels, of the Silverlight plug-in content area. The default value is the width of the Silverlight plug-in, as specified by the HTML object element that instantiated it. |
| `FullScreenOptions` | Gets or sets a value that indicates the behavior of full-screen mode. <br>Returns: A value that indicates the behavior of full-screen mode. |
| `IsFullScreen` | Gets or sets a value that indicates whether the Silverlight plug-in is displaying in full-screen mode. <br>Returns: true if the Silverlight plug-in displays as a full-screen plug-in. false if the Silverlight plug-in displays as an embedded plug-in. |
| `ZoomFactor` | Gets the factor by which the current browser window resizes its contents. <br>Returns: The zoom setting for the current browser window. |

### Events

| Member | Description |
|---|---|
| `FullScreenChanged` | Occurs when the hosted Silverlight plug-in either enters or exits full-screen mode. |
| `Resized` | Occurs when the `ActualHeight` or the `ActualWidth` of the Silverlight plug-in change. |
| `Zoomed` | Occurs when the zoom setting in the host browser window changes or is initialized. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Content` class. |

## System.Windows.Interop.FullScreenOptions

Defines constants that indicate the behavior of full-screen mode.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `None` | The application uses the default full-screen behavior. |
| `StaysFullScreenWhenUnfocused` | The application does not exit full-screen mode when other applications gain focus. |

## System.Windows.Interop.HostingRenderTargetBitmap

Enables a native Silverlight host, such as Expression Blend or Visual Studio, to convert a `UIElement` object into a bitmap.

### Methods

| Member | Description |
|---|---|
| `Constructor(Int32,Int32,IntPtr)` | [SECURITY CRITICAL] Initializes a new instance of the `HostingRenderTargetBitmap` class. <br>`width`: The width of the `UIElement` to convert. <br>`height`: The height of the `UIElement` to convert. <br>`bitmap`: A pointer to the target bitmap. |
| `Render(UIElement)` | Renders the `UIElement` object to the bitmap pointer specified in the constructor. <br>`visual`: The object to render. <br>Returns: The dirty rectangle. |

## System.Windows.Interop.NavigationStateChangedEventArgs

Provides data for the `NavigationStateChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `NewNavigationState` | Gets the URI fragment that represents the new navigation state. <br>Returns: The URI fragment that represents the new navigation state. |
| `PreviousNavigationState` | Gets the URI fragment that represents the previous navigation state. <br>Returns: The URI fragment that represents the previous navigation state. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String,String)` | Initializes a new instance of the `NavigationStateChangedEventArgs` class. <br>`previousNavigationState`: The URI fragment that represents the previous navigation state. <br>`newNavigationState`: The URI fragment that represents the new navigation state. |

## System.Windows.Interop.Settings

Reports details about the Silverlight-based application's instantiation settings, and exposes some of the HTML DOM values for the hosted Silverlight plug-in instance.

### Properties / fields

| Member | Description |
|---|---|
| `EnableAutoZoom` | Gets or sets a value that indicates whether the Silverlight plug-in will resize its content based on the current browser zoom setting. <br>Returns: true if Silverlight responds to the browser zoom setting; otherwise, false. The default is true if there is no handler for the `Zoomed` event; otherwise, the default is false. |
| `EnableCacheVisualization` | Gets or sets a value that indicates whether to use a non-production analysis visualization mode, which shows areas of a page that are not being GPU accelerated with a colored overlay. Do not use in production code. <br>Returns: true if cache visualization is enabled; otherwise, false. |
| `EnableFrameRateCounter` | Gets or sets a value that indicates whether to display the current frame rate in the hosting browser's status bar. (Microsoft Internet Explorer only.) <br>Returns: true if the frames-per-second (fps) of the current rendered Silverlight content will be displayed in the hosting browser's status bar; otherwise, false. The default is false. |
| `EnableGPUAcceleration` | Gets a value that indicates whether to use graphics processor unit (GPU) hardware acceleration for cached compositions, which potentially results in graphics optimization. <br>Returns: true if hardware acceleration is enabled; otherwise, false. |
| `EnableHTMLAccess` | Gets a value that indicates whether the Silverlight plug-in allows hosted content or its runtime to access the HTML DOM. <br>Returns: true if hosted content can access the browser DOM; otherwise, false. |
| `EnableRedrawRegions` | Gets or sets a value that indicates whether to show the areas of the Silverlight plug-in that are being redrawn each frame. <br>Returns: true if the areas of the plug-in that are being redrawn each frame are shown; otherwise, false. |
| `MaxFrameRate` | Gets or sets the maximum number of frames to render per second. <br>Returns: An integer value that specifies the maximum number of frames to render per second. The default value is 60. |
| `Windowless` | Gets a value that indicates whether the Silverlight plug-in displays as a windowless plug-in. (Applies to Windows versions of Silverlight only.) <br>Returns: true if the Silverlight plug-in displays as a windowless plug-in; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Settings` object. |

## System.Windows.Interop.SilverlightHost

Reports details about the Silverlight-based application's instantiation settings, and exposes some of the HTML DOM values for the hosted Silverlight plug-in instance.

### Properties / fields

| Member | Description |
|---|---|
| `Background` | Gets the background color value that was applied to the Silverlight plug-in as part of instantiation settings. <br>Returns: The background color for the Silverlight plug-in. |
| `Content` | Gets the "Content" sub-object of this `SilverlightHost`. <br>Returns: The "Content" sub-object. |
| `InitParams` | Gets the initialization parameters that were passed as part of HTML initialization of a Silverlight plug-in. <br>Returns: The set of initialization parameters, as a dictionary with key strings and value strings. |
| `IsLoaded` | Gets a value that indicates whether the hosted Silverlight plug-in has finished loading. <br>Returns: true if the plug-in has finished loading; otherwise, false. |
| `NavigationState` | Gets or sets a URI fragment that represents the current navigation state. <br>Returns: A URI fragment that represents the current navigation state. |
| `Settings` | Gets the "Settings" sub-object of this `SilverlightHost`. <br>Returns: The "Settings" sub-object. |
| `Source` | Gets the URI of the package or XAML file that specifies the XAML content to render. <br>Returns: The URI of the package, XAML file, or XAML scripting tag that contains the content to load into the Silverlight plug-in. |

### Events

| Member | Description |
|---|---|
| `NavigationStateChanged` | Occurs when the `NavigationState` property changes value. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SilverlightHost` class. |
| `IsVersionSupported(String)` | Returns a value that indicates whether the installed Silverlight plug-in supports the specified version. <br>`versionStr`: The version to check, in the form of major.minor.build.revision See Remarks for more information about the string form. <br>Returns: true if the version can be supported by the installation; otherwise, false. |

## System.Windows.LayoutCycleException

An exception that is thrown by the layout cycle.

## System.Windows.LineStackingStrategy

Describes the mechanism by which a line box is determined for each line.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BaselineToBaseline` | The stack height is determined by adding LineHeight to the baseline of the previous line. |
| `BlockLineHeight` | The stack height is determined by the block element line-height property value. |
| `MaxHeight` | The stack height is the smallest value that contains the extended block progression dimension of all the inline elements on that line when those elements are properly aligned. This is the default. |

## System.Windows.Markup.ContentPropertyAttribute

Specifies which property of a class can be interpreted to be the content property when the class is parsed by a XAML processor.

### Properties / fields

| Member | Description |
|---|---|
| `Name` | Gets the name of the property that is the declared content property. <br>Returns: A string that contains the name of the property that is the content property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ContentPropertyAttribute` class. |
| `Constructor(String)` | Initializes a new instance of the `ContentPropertyAttribute` class, using the specified content property name. <br>`name`: The content property name. |

## System.Windows.Markup.IProvideValueTarget

Represents a service that reports situational object-property relationships for markup extension evaluation.

### Properties / fields

| Member | Description |
|---|---|
| `TargetObject` | Gets the target object being reported. <br>Returns: The target object being reported. |
| `TargetProperty` | Gets an identifier for the target property being reported. <br>Returns: An identifier for the target property being reported. |

## System.Windows.Markup.IXamlTypeResolver

Represents a service that resolves from named elements in XAML markup to the appropriate CLR type.

### Methods

| Member | Description |
|---|---|
| `Resolve(String)` | Resolves a named XAML type to the corresponding CLR `Type`. <br>`qualifiedTypeName`: The XAML type name to resolve. The type name is optionally qualified by the prefix for a XAML namespace. Otherwise the current default XAML namespace is assumed. <br>Returns: The`Type` that `qualifiedTypeName` resolves to. |

## System.Windows.Markup.MarkupExtension

Provides a base class for XAML markup extension implementations that can be supported by Silverlight XAML processors.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `MarkupExtension` class. |
| `ProvideValue(IServiceProvider)` | When implemented in a derived class, returns an object that is provided as the value of the target property for this markup extension. <br>`serviceProvider`: A service provider helper that can provide services for the markup extension. <br>Returns: The object value to set on the property where the extension is applied. |

## System.Windows.Markup.XamlParseException

The exception that is thrown when a XAML parsing exception occurs.

### Properties / fields

| Member | Description |
|---|---|
| `LineNumber` | Gets the line number where the exception occurred. <br>Returns: The number of the line on which the XAML parsing exception occurred. |
| `LinePosition` | Gets the position in the line where the exception occurred. <br>Returns: The position on the line where the XAML parsing exception occurred. |

## System.Windows.Markup.XamlReader

Provides a XAML processor engine for parsing XAML and creating corresponding Silverlight object trees.

### Methods

| Member | Description |
|---|---|
| `Load(String)` | Parses a well-formed XAML fragment and creates a corresponding Silverlight object tree, and returns the root of the object tree. <br>`xaml`: A string that contains a valid XAML fragment. <br>Returns: The root object of the Silverlight object tree. |
| `LoadWithInitialTemplateValidation(String)` | [SECURITY CRITICAL] Cannot be called. <br>`xaml`: Cannot be called. <br>Returns: Cannot be called. |

## System.Windows.Markup.XmlLanguage

Represents culture information for use in XML and XAML markup.

### Properties / fields

| Member | Description |
|---|---|
| `IetfLanguageTag` | Gets the string representation of the language tag. <br>Returns: A string that represents the language tag. |

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Checks for equality of an object to the current object. <br>`obj`: The `Object` to compare to the calling object. <br>Returns: true if the objects are the same object; otherwise, false. |
| `GetHashCode` | Serves as a hash function for the `XmlLanguage` class. <br>Returns: An integer hash value. |
| `GetLanguage(String)` | Returns a `XmlLanguage` instance, based on a string representing the language per RFC 3066. <br>`ietfLanguageTag`: An RFC 3066 language string, or an empty string (""). <br>Returns: A new `XmlLanguage` with the provided string as its `IetfLanguageTag` value. |
| `op_Equality(Markup.XmlLanguage,Markup.XmlLanguage)` | Determines whether two specified `XmlLanguage` objects have the same value. <br>`xmlLanguage1`: The first `XmlLanguage` to compare. <br>`xmlLanguage2`: The second `XmlLanguage` to compare. <br>Returns: true if the value of is the same as the value of ; otherwise, false. |
| `op_Inequality(Markup.XmlLanguage,Markup.XmlLanguage)` | Determines whether two specified `XmlLanguage` objects have a different value. <br>`xmlLanguage1`: The first `XmlLanguage` to compare. <br>`xmlLanguage2`: The second `XmlLanguage` to compare. <br>Returns: true if the value of is different from the value of ; otherwise, false. |

## System.Windows.Markup.XmlnsDefinitionAttribute

Specifies a mapping in an assembly between an XML namespace identifier and a CLR namespace within that assembly.

### Properties / fields

| Member | Description |
|---|---|
| `AssemblyName` | Gets or sets the name of the assembly that contains the `ClrNamespace`. <br>Returns: The name of the assembly that contains the `ClrNamespace`. |
| `ClrNamespace` | Gets the CLR namespace that is mapped to `XmlNamespace` by the attribute. <br>Returns: The CLR namespace that is mapped to `XmlNamespace` by the attribute. |
| `XmlNamespace` | Gets the XML namespace (xmlns) that is mapped to `ClrNamespace` by the attribute. <br>Returns: The XML namespace (xmlns) that is mapped to `ClrNamespace` by the attribute. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String,String)` | Initializes a new instance of the `XmlnsDefinitionAttribute` class. <br>`xmlNamespace`: Specifies the `XmlNamespace` value. <br>`clrNamespace`: Specifies the `ClrNamespace` value. |

## System.Windows.Markup.XmlnsPrefixAttribute

Specifies a recommended prefix to associate with an XML namespace when writing elements and attributes in a XAML file.

### Properties / fields

| Member | Description |
|---|---|
| `Prefix` | Gets the recommended prefix to associate with the XML namespace. <br>Returns: A string that provides the recommended prefix to use when mapping the `XmlNamespace` namespace. |
| `XmlNamespace` | Gets the XML namespace identifier. <br>Returns: A string that provides the identifier for the relevant XML namespace. By convention, this is often a URI string. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String,String)` | Initializes a new instance of the `XmlnsPrefixAttribute` class. <br>`xmlNamespace`: The identifier of the relevant XML namespace (use a string, not a true `Uri` type). <br>`prefix`: The recommended prefix to use when mapping the XML namespace. |

## System.Windows.Media.AcquireLicenseCompletedEventArgs

Provides data for the `AcquireLicenseCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `ResponseCustomData` | Gets any custom data returned from the server in the license response. <br>Returns: Any custom data returned from the server in the license response. If there is no custom data to return, you get an empty string. |

## System.Windows.Media.AlignmentX

Describes how content is positioned horizontally in a container.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Center` | The contents align toward the center of the container. |
| `Left` | The contents align toward the left of the container. |
| `Right` | The contents align toward the right of the container. |

## System.Windows.Media.AlignmentY

Describes how content is positioned vertically in a container.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Bottom` | The contents align toward the lower edge of the container. |
| `Center` | The contents align toward the center of the container. |
| `Top` | The contents align toward the upper edge of the container. |

## System.Windows.Media.Animation.BackEase

Represents an easing function that retracts the motion of an animation slightly before it begins to animate in the path indicated.

### Properties / fields

| Member | Description |
|---|---|
| `Amplitude` | Gets or sets the amplitude of retraction associated with a `BackEase` animation. <br>Returns: The amplitude of retraction associated with a `BackEase` animation. This value must be greater than or equal to 0. The default is 1. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AmplitudeProperty` | Identifies the `Amplitude` dependency property. <br>Returns: The identifier for the `Amplitude` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BackEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.BeginStoryboard

A trigger action that begins a `Storyboard` and distributes its animations to their targeted objects and properties.

### Properties / fields

| Member | Description |
|---|---|
| `Storyboard` | Gets or sets the `Storyboard` that this `BeginStoryboard` starts. <br>Returns: The `Storyboard` that the `BeginStoryboard` starts. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `StoryboardProperty` | Identifies the `Storyboard` dependency property. <br>Returns: The identifier for the `Storyboard` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BeginStoryboard` class. |

## System.Windows.Media.Animation.BounceEase

Represents an easing function that creates an animated bouncing effect.

### Properties / fields

| Member | Description |
|---|---|
| `Bounces` | Gets or sets the number of bounces. <br>Returns: The number of bounces. The value must be greater or equal to zero. Negative values will resolve to zero. The default is 3. |
| `Bounciness` | Gets or sets a value that specifies how bouncy the bounce animation is. Low values of this property result in bounces with little lose of height between bounces (more bouncy) while high values result in dampened bounces (less bouncy). <br>Returns: The value that specifies how bouncy the bounce animation is. This value must be positive. The default value is 2. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BouncesProperty` | Identifies the `Bounces` dependency property. |
| `BouncinessProperty` | Identifies the `Bounciness` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BounceEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.CircleEase

Represents an easing function that creates an animation that accelerates and/or decelerates using a circular function.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CircleEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.ClockState

Describes the potential states of an animation.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Active` | The current animation changes in direct relation to that of its parent. |
| `Filling` | The animation continues and does not change in relation to that of its parent. |
| `Stopped` | The animation is stopped. |

## System.Windows.Media.Animation.ColorAnimation

Animates the value of a `Color` property between two target values using linear interpolation over a specified `Duration`.

### Properties / fields

| Member | Description |
|---|---|
| `By` | Gets or sets the total amount by which the animation changes its starting value. <br>Returns: The total amount by which the animation changes its starting value. The default is null. |
| `EasingFunction` | Gets or sets the easing function applied to this animation. <br>Returns: The easing function applied to this animation. |
| `From` | Gets or sets the animation's starting value. <br>Returns: The starting value of the animation. The default is null. |
| `To` | Gets or sets the animation's ending value. <br>Returns: The ending value of the animation. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ByProperty` | Identifies the `By` dependency property. <br>Returns: The identifier for the `By` dependency property. |
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |
| `FromProperty` | Identifies the `From` dependency property. <br>Returns: The identifier for the `From` dependency property. |
| `ToProperty` | Identifies the `To` dependency property. <br>Returns: The identifier for the `To` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ColorAnimation` class. |

## System.Windows.Media.Animation.ColorAnimationUsingKeyFrames

Animates the value of a `Color` property along a set of `ZeyFrames` over a specified `Duration`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyFrames` | Gets the collection of `ColorKeyFrame` objects that define the animation. <br>Returns: The collection of `ColorKeyFrame` objects that define the animation. The default is an empty collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ColorAnimationUsingKeyFrames` class. |

## System.Windows.Media.Animation.ColorKeyFrame

Provides a base class for specific animation key-frame techniques that define an animation segment with a `Color` target value. Derived classes each provide a different key-frame interpolation method for a `Color` value that is provided for a `ColorAnimationUsingKeyFrames` animation.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyTime` | Gets or sets the time at which the key frame's target `Value` should be reached. <br>Returns: The time at which the key frame's current value should be equal to its `Value` property. The default is null. |
| `Value` | Gets or sets the key frame's target value. <br>Returns: The key frame's target value, which is the value at its specified `ZeyTime`. The default is a `Color` with an ARGB value of #00000000. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeyTimeProperty` | Identifies the `ZeyTime` dependency property. <br>Returns: The identifier for the `ZeyTime` dependency property. |
| `ValueProperty` | Identifies the `Value` dependency property. <br>Returns: The identifier for the `Value` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ColorKeyFrame` class. |

## System.Windows.Media.Animation.ColorKeyFrameCollection

Represents a collection of `ColorKeyFrame` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ColorKeyFrameCollection` class. |

## System.Windows.Media.Animation.CubicEase

Represents an easing function that creates an animation that accelerates and/or decelerates using the formula f(t) = t3.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CubicEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.DiscreteColorKeyFrame

Animates from the `Color` value of the previous key frame to its own `Value` using discrete values.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DiscreteColorKeyFrame` class. |

## System.Windows.Media.Animation.DiscreteDoubleKeyFrame

Animates from the `Double` value of the previous key frame to its own `Value` using discrete values.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DiscreteDoubleKeyFrame` class. |

## System.Windows.Media.Animation.DiscreteObjectKeyFrame

Animates from the `Object` value of the previous key frame to its own `Value` using discrete values.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DiscreteObjectKeyFrame` class. |

## System.Windows.Media.Animation.DiscretePointKeyFrame

Animates from the `Point` value of the previous key frame to its own `Value` using discrete frames.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DiscretePointKeyFrame` class. |

## System.Windows.Media.Animation.DoubleAnimation

Animates the value of a `Double` property between two target values using linear interpolation over a specified `Duration`.

### Properties / fields

| Member | Description |
|---|---|
| `By` | Gets or sets the total amount by which the animation changes its starting value. <br>Returns: The total amount by which the animation changes its starting value. The default is null. |
| `EasingFunction` | Gets or sets the easing function applied to this animation. <br>Returns: The easing function applied to this animation. |
| `From` | Gets or sets the animation's starting value. <br>Returns: The starting value of the animation. The default is null. |
| `To` | Gets or sets the animation's ending value. <br>Returns: The ending value of the animation. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ByProperty` | Identifies the `By` dependency property. <br>Returns: The identifier for the `By` dependency property. |
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |
| `FromProperty` | Identifies the `From` dependency property. <br>Returns: The identifier for the `From` dependency property. |
| `ToProperty` | Identifies the `To` dependency property. <br>Returns: The identifier for the `To` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DoubleAnimation` class. |

## System.Windows.Media.Animation.DoubleAnimationUsingKeyFrames

Animates the value of a `Double` property along a set of `ZeyFrames`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyFrames` | Gets the collection of `DoubleKeyFrame` objects that define the animation. <br>Returns: The collection of `DoubleKeyFrame` objects that define the animation. The default is an empty collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DoubleAnimationUsingKeyFrames` class. |

## System.Windows.Media.Animation.DoubleKeyFrame

An abstract class that defines an animation segment with its own target value and interpolation method for a `DoubleAnimationUsingKeyFrames`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyTime` | Gets or sets the time at which the key frame's target `Value` should be reached. <br>Returns: The time at which the key frame's current value should be equal to its `Value` property. The default is null. |
| `Value` | Gets or sets the key frame's target value. <br>Returns: The key frame's target value, which is the value of this key frame at its specified `ZeyTime`. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeyTimeProperty` | Identifies the `ZeyTime` dependency property. <br>Returns: The identifier for the `ZeyTime` dependency property. |
| `ValueProperty` | Identifies the `Value` dependency property. <br>Returns: The identifier for the `Value` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DoubleKeyFrame` class. |

## System.Windows.Media.Animation.DoubleKeyFrameCollection

Represents a collection of `DoubleKeyFrame` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DoubleKeyFrameCollection` class. |

## System.Windows.Media.Animation.EasingColorKeyFrame

A class that enables you to associate easing functions with a `ColorAnimationUsingKeyFrames` key frame animation.

### Properties / fields

| Member | Description |
|---|---|
| `EasingFunction` | Gets or sets the easing function that is applied to the key frame. <br>Returns: The easing function that is applied to the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EasingColorKeyFrame` class. |

## System.Windows.Media.Animation.EasingDoubleKeyFrame

Defines a property that enables you to associate an easing function with a `DoubleAnimationUsingKeyFrames` key-frame animation.

### Properties / fields

| Member | Description |
|---|---|
| `EasingFunction` | Gets or sets the easing function that is applied to the key frame. <br>Returns: The easing function that is applied to the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EasingDoubleKeyFrame` class. |

## System.Windows.Media.Animation.EasingFunctionBase

Provides the base class for all the easing functions. You can create your own custom easing functions by inheriting from this class.

### Properties / fields

| Member | Description |
|---|---|
| `EasingMode` | Gets or sets a value that specifies how the animation interpolates. <br>Returns: One of the enumeration values that specifies how the animation interpolates. The default is `EaseOut`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EasingModeProperty` | Identifies the `EasingMode` dependency property. <br>Returns: The identifier for the `EasingMode` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EasingFunctionBase` class. |
| `Ease(Double)` | Transforms normalized time to control the pace of an animation. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.EasingMode

Specifies how the animation associated with an easing function interpolates.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EaseIn` | Interpolation follows the mathematical formula associated with the easing function. |
| `EaseInOut` | Interpolation uses `EaseIn` for the first half of the animation and `EaseOut` for the second half. |
| `EaseOut` | Interpolation follows 100% interpolation minus the output of the formula associated with the easing function. |

## System.Windows.Media.Animation.EasingPointKeyFrame

Defines a property that enables you to associate an easing function with a `PointAnimationUsingKeyFrames` key-frame animation.

### Properties / fields

| Member | Description |
|---|---|
| `EasingFunction` | Gets or sets the easing function that is applied to the key frame. <br>Returns: The easing function that is applied to the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EasingPointKeyFrame` class. |

## System.Windows.Media.Animation.ElasticEase

Represents an easing function that creates an animation that resembles a spring oscillating back and forth until it comes to rest.

### Properties / fields

| Member | Description |
|---|---|
| `Oscillations` | Gets or sets the number of times the target slides back and forth over the animation destination. <br>Returns: The number of times the target slides back and forth over the animation destination. This value must be greater than or equal to 0. The default is 3. |
| `Springiness` | Gets or sets the stiffness of the spring. The smaller the Springiness value is, the stiffer the spring and the faster the elasticity decreases in intensity over each oscillation. <br>Returns: A positive number that specifies the stiffness of the spring. The default value is 3. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `OscillationsProperty` | Identifies the `Oscillations` dependency property. <br>Returns: The identifier for the `Oscillations` dependency property. |
| `SpringinessProperty` | Identifies the `Springiness` dependency property. <br>Returns: The identifier for the `Springiness` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ElasticEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.ExponentialEase

Represents an easing function that creates an animation that accelerates and/or decelerates using an exponential formula (see remarks).

### Properties / fields

| Member | Description |
|---|---|
| `Exponent` | Gets or sets the exponent used to determine the interpolation of the animation. <br>Returns: The exponent used to determine the interpolation of the animation. The default is 2. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ExponentProperty` | Identifies the `Exponent` dependency property. <br>Returns: The identifier for the `Exponent` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ExponentialEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.FillBehavior

Specifies how a `Timeline` behaves when it is outside its active period but its parent is inside its active or hold period.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `HoldEnd` | After it reaches the end of its active period, the timeline holds its progress until the end of its parent's active and hold periods. |
| `Stop` | The timeline stops if it is outside its active period while its parent is inside its active period. |

## System.Windows.Media.Animation.IEasingFunction

Defines the basic functionality of an easing function.

### Methods

| Member | Description |
|---|---|
| `Ease(Double)` | Transforms normalized time to control the pace of an animation. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: The transformed progress. |

## System.Windows.Media.Animation.ZeySpline

This class is used by a spline key frame to define animation progress.

### Properties / fields

| Member | Description |
|---|---|
| `ControlPoint1` | Gets or sets the first control point used to define a Bezier curve that describes a `ZeySpline`. <br>Returns: The first control point used to define a Bezier curve that describes a `ZeySpline`. |
| `ControlPoint2` | Gets or sets the second control point used to define a Bezier curve that describes a `ZeySpline`. <br>Returns: The second control point used to define a Bezier curve that describes a `ZeySpline`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ZeySpline` class. |

## System.Windows.Media.Animation.ZeyTime

Specifies when a particular key frame should take place during an animation.

### Properties / fields

| Member | Description |
|---|---|
| `TimeSpan` | Gets the time when the key frame ends, expressed as a time relative to the beginning of the animation. <br>Returns: The time when the key frame ends, expressed as a time relative to the beginning of the animation. |
| `Type` | Gets the `ZeyTimeType` value this instance represents. <br>Returns: One of the enumeration values. |
| `Uniform` | Gets a uniform value, which divides the allotted time of the animation evenly between key frames. <br>Returns: A uniform value, which divides the allotted time of the animation evenly between key frames. |

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Indicates whether a `ZeyTime` is equal to this `ZeyTime`. <br>`value`: The `ZeyTime` to compare with this `ZeyTime`. <br>Returns: true if `value` is a `ZeyTime` that represents the same length of time as this `ZeyTime`; otherwise, false. |
| `Equals(Media.Animation.ZeyTime)` | Indicates whether a specified `ZeyTime` is equal to this `ZeyTime`. <br>`value`: The `ZeyTime` to compare with this `ZeyTime`. <br>Returns: true if `value` is equal to this `ZeyTime`; otherwise, false. |
| `Equals(Media.Animation.ZeyTime,Media.Animation.ZeyTime)` | Indicates whether two `ZeyTime` values are equal. <br>`keyTime1`: The first value to compare. <br>`keyTime2`: The second value to compare. <br>Returns: true if the values of and are equal; otherwise, false. |
| `FromTimeSpan(TimeSpan)` | Creates a new `ZeyTime`, using the supplied `TimeSpan`. <br>`timeSpan`: The value of the new `ZeyTime`. <br>Returns: A new `ZeyTime`, initialized to the value of `timeSpan`. |
| `GetHashCode` | Returns a hash code representing this `ZeyTime`. <br>Returns: A hash code identifier. |
| `ToString` | Returns a string representation of this `ZeyTime`. <br>Returns: A string representation of this `ZeyTime`. |
| `op_Equality(Media.Animation.ZeyTime,Media.Animation.ZeyTime)` | Compares two `ZeyTime` values for equality. <br>`keyTime1`: The first value to compare. <br>`keyTime2`: The second value to compare. <br>Returns: true if and are equal; otherwise, false. |
| `op_Implicit(TimeSpan)` | Implicitly converts a `TimeSpan` to a `ZeyTime`. <br>`timeSpan`: The `TimeSpan` value to convert. <br>Returns: The created `ZeyTime`. |
| `op_Inequality(Media.Animation.ZeyTime,Media.Animation.ZeyTime)` | Compares two `ZeyTime` values for inequality. <br>`keyTime1`: The first value to compare. <br>`keyTime2`: The second value to compare. <br>Returns: true if and are not equal; otherwise, false. |

## System.Windows.Media.Animation.ZeyTimeType

Represents the different types that may represent a `ZeyTime` instance.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `TimeSpan` | Each `ZeyTime` is expressed as a `TimeSpan` value relative to the `BeginTime` of an animation sequence. |
| `Uniform` | The allotted total time for an animation sequence is divided evenly amongst each of the key frames. |

## System.Windows.Media.Animation.LinearColorKeyFrame

Animates from the `Color` value of the previous key frame to its own `Value` using linear interpolation.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LinearColorKeyFrame` class. |

## System.Windows.Media.Animation.LinearDoubleKeyFrame

Animates from the `Double` value of the previous key frame to its own `Value` using linear interpolation.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LinearDoubleKeyFrame` class. |

## System.Windows.Media.Animation.LinearPointKeyFrame

Animates from the `Point` value of the previous key frame to its own `Value` using linear interpolation.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LinearPointKeyFrame` class. |

## System.Windows.Media.Animation.ObjectAnimationUsingKeyFrames

Animates the value of an `Object` property along a set of `ZeyFrames` over a specified `Duration`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyFrames` | Gets the collection of `ObjectKeyFrame` objects that define the animation. <br>Returns: The collection of `ObjectKeyFrame` objects that define the animation. The default is an empty collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ObjectAnimationUsingKeyFrames` class. |

## System.Windows.Media.Animation.ObjectKeyFrame

Defines an animation segment with its own target value and interpolation method for an `ObjectAnimationUsingKeyFrames`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyTime` | Gets or sets the time at which the key frame's target `Value` should be reached. <br>Returns: The time at which the key frame's current value should be equal to its `Value` property. The default is null. |
| `Value` | Gets or sets the key frame's target value. <br>Returns: The key frame's target value, which is the value of this key frame at its specified `ZeyTime`. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeyTimeProperty` | Identifies the `ZeyTime` dependency property. <br>Returns: The identifier for the `ZeyTime` dependency property. |
| `ValueProperty` | Identifies the `Value` dependency property. <br>Returns: The identifier for the `Value` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ObjectKeyFrame` class. |

## System.Windows.Media.Animation.ObjectKeyFrameCollection

Represents a collection of `ObjectKeyFrame` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ObjectKeyFrameCollection` class. |

## System.Windows.Media.Animation.PointAnimation

Animates the value of a `Point` property between two target values using linear interpolation over a specified `Duration`.

### Properties / fields

| Member | Description |
|---|---|
| `By` | Gets or sets the total amount by which the animation changes its starting value. <br>Returns: The total amount by which the animation changes its starting value. The default is null. |
| `EasingFunction` | Gets or sets the easing function you are applying to the animation. <br>Returns: The easing function you are applying to the animation. The default is null. |
| `From` | Gets or sets the animation's starting value. <br>Returns: The starting value of the animation. The default is null. |
| `To` | Gets or sets the animation's ending value. <br>Returns: The ending value of the animation. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ByProperty` | Identifies the `By` dependency property. <br>Returns: The identifier for the `By` dependency property. |
| `EasingFunctionProperty` | Identifies the `EasingFunction` dependency property. <br>Returns: The identifier for the `EasingFunction` dependency property. |
| `FromProperty` | Identifies the `From` dependency property. <br>Returns: The identifier for the `From` dependency property. |
| `ToProperty` | Identifies the `To` dependency property. <br>Returns: The identifier for the `To` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PointAnimation` class. |

## System.Windows.Media.Animation.PointAnimationUsingKeyFrames

Animates the value of a `Point` property along a set of `ZeyFrames`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyFrames` | Gets the collection of `PointKeyFrame` objects that define the animation. <br>Returns: The collection of `PointKeyFrame` objects that define the animation. The default is an empty collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PointAnimationUsingKeyFrames` class. |

## System.Windows.Media.Animation.PointKeyFrame

Defines an animation segment with its own target value and interpolation method for a `PointAnimationUsingKeyFrames`.

### Properties / fields

| Member | Description |
|---|---|
| `ZeyTime` | Gets or sets the time at which the key frame's target `Value` should be reached. <br>Returns: The time at which the key frame's current value should be equal to its `Value` property. The default is null. |
| `Value` | Gets or sets the key frame's target value. <br>Returns: The key frame's target value, which is the value of this key frame at its specified `ZeyTime`. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeyTimeProperty` | Identifies the `ZeyTime` dependency property. <br>Returns: The identifier for the `ZeyTime` dependency property. |
| `ValueProperty` | Identifies the `Value` dependency property. <br>Returns: The identifier for the `Value` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PointKeyFrame` class. |

## System.Windows.Media.Animation.PointKeyFrameCollection

Represents a collection of `PointKeyFrame` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PointKeyFrameCollection` class. |

## System.Windows.Media.Animation.PowerEase

Represents an easing function that creates an animation that accelerates and/or decelerates using the formula f(t) = tp where p is equal to the `Power` property.

### Properties / fields

| Member | Description |
|---|---|
| `Power` | Gets or sets the exponential power of the animation interpolation. For example, a value of 7 creates an animation interpolation curve that follows the formula f(t) = t7. <br>Returns: The exponential power of the animation interpolation. This value must be greater or equal to 0. The default is 2. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PowerProperty` | Identifies the `Power` dependency property. <br>Returns: The identifier for the `Power` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PowerEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.QuadraticEase

Represents an easing function that creates an animation that accelerates and/or decelerates using the formula f(t) = t2

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `QuadraticEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.QuarticEase

Represents an easing function that creates an animation that accelerates and/or decelerates using the formula f(t) = t4.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `QuarticEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.QuinticEase

Represents an easing function that creates an animation that accelerates and/or decelerates using the formula f(t) = t5.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `QuinticEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.RepeatBehavior

Describes how a `Timeline` repeats its simple duration.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of times a `Timeline` should repeat. <br>Returns: The number of iterations to repeat. |
| `Duration` | Gets the total length of time a `Timeline` should play. <br>Returns: The total length of time a timeline should play. |
| `Forever` | Gets a `RepeatBehavior` that specifies an infinite number of repetitions. <br>Returns: A `RepeatBehavior` that specifies an infinite number of repetitions. |
| `HasCount` | Gets a value that indicates whether the repeat behavior has a specified iteration count. <br>Returns: true if the instance represents an iteration count; otherwise, false. |
| `HasDuration` | Gets a value that indicates whether the repeat behavior has a specified repeat duration. <br>Returns: true if the instance represents a repeat duration; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double)` | Initializes a new instance of the `RepeatBehavior` structure with the specified iteration count. <br>`count`: A number greater than or equal to 0 that specifies the number of iterations for an animation. |
| `Constructor(TimeSpan)` | Initializes a new instance of the `RepeatBehavior` structure with the specified repeat duration. <br>`duration`: The total length of time that the `Timeline` should play (its active duration). |
| `Equals(Object)` | Indicates whether the specified object is equal to this `RepeatBehavior`. <br>`value`: The object to compare with this `RepeatBehavior`. <br>Returns: true if `value` is a `RepeatBehavior` that represents the same repeat behavior as this `RepeatBehavior`; otherwise, false. |
| `Equals(Media.Animation.RepeatBehavior)` | Returns a value that indicates whether the specified `RepeatBehavior` is equal to this `RepeatBehavior`. <br>`repeatBehavior`: The value to compare to this `RepeatBehavior`. <br>Returns: true if both the type and repeat behavior of `repeatBehavior` are equal to this `RepeatBehavior`; otherwise, false. |
| `Equals(Media.Animation.RepeatBehavior,Media.Animation.RepeatBehavior)` | Indicates whether the two specified `RepeatBehavior` values are equal. <br>`repeatBehavior1`: The first value to compare. <br>`repeatBehavior2`: The second value to compare. <br>Returns: true if both the type and repeat behavior of are equal to that of ; otherwise, false. |
| `GetHashCode` | Returns the hash code of this instance. <br>Returns: A hash code. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `IFormatProvider)`. <br>`format`: The string specifying the format to use, or null to use the default format defined for the type of the IFormattable implementation. <br>`formatProvider`: The IFormatProvider to use to format the value, or null to obtain the numeric format information from the current locale setting of the operating system. <br>Returns: A string containing the value of the current instance in the specified format. |
| `ToString` | Returns a string representation of this `RepeatBehavior`. <br>Returns: A string representation of this `RepeatBehavior`. |
| `ToString(IFormatProvider)` | Returns a string representation of this `RepeatBehavior` with the specified format. <br>`formatProvider`: The format used to construct the return value. <br>Returns: A string representation of this `RepeatBehavior`. |
| `op_Equality(Media.Animation.RepeatBehavior,Media.Animation.RepeatBehavior)` | Indicates whether the two specified `RepeatBehavior` values are equal. <br>`repeatBehavior1`: The first value to compare. <br>`repeatBehavior2`: The second value to compare. <br>Returns: true if both the type and repeat behavior of are equal to that of ; otherwise, false. |
| `op_Inequality(Media.Animation.RepeatBehavior,Media.Animation.RepeatBehavior)` | Indicates whether the two `RepeatBehavior` values are not equal. <br>`repeatBehavior1`: The first value to compare. <br>`repeatBehavior2`: The second value to compare. <br>Returns: true if and are different types or the repeat behavior properties are not equal; otherwise, false. |

## System.Windows.Media.Animation.SineEase

Represents an easing function that creates an animation that accelerates and/or decelerates using a sine formula (see remarks).

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SineEase` class. |
| `EaseInCore(Double)` | Provides the logic portion of the easing function that you can override to produce the `EaseIn` mode of the custom easing function. <br>`normalizedTime`: Normalized time (progress) of the animation. <br>Returns: A double that represents the transformed progress. |

## System.Windows.Media.Animation.SplineColorKeyFrame

Animates from the `Color` value of the previous key frame to its own `Value` using splined interpolation.

### Properties / fields

| Member | Description |
|---|---|
| `ZeySpline` | Gets or sets the two control points that define animation progress for this key frame. <br>Returns: The two control points that specify the cubic Bezier curve which defines the progress of the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeySplineProperty` | Identifies the `ZeySpline` dependency property. <br>Returns: The identifier for the `ZeySpline` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SplineColorKeyFrame` class. |

## System.Windows.Media.Animation.SplineDoubleKeyFrame

Animates from the `Double` value of the previous key frame to its own `Value` using splined interpolation.

### Properties / fields

| Member | Description |
|---|---|
| `ZeySpline` | Gets or sets the two control points that define animation progress for this key frame. <br>Returns: The two control points that specify the cubic Bezier curve which defines the progress of the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeySplineProperty` | Identifies the `ZeySpline` dependency property. <br>Returns: The identifier for the `ZeySpline` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SplineDoubleKeyFrame` class. |

## System.Windows.Media.Animation.SplinePointKeyFrame

Animates from the `Point` value of the previous key frame to its own `Value` using splined interpolation.

### Properties / fields

| Member | Description |
|---|---|
| `ZeySpline` | Gets or sets the two control points that define animation progress for this key frame. <br>Returns: The two control points that specify the cubic Bezier curve that defines the progress of the key frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ZeySplineProperty` | Identifies the `ZeySpline` dependency property. <br>Returns: The identifier for the `ZeySpline` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SplinePointKeyFrame` class. |

## System.Windows.Media.Animation.Storyboard

Controls animations with a timeline, and provides object and property targeting information for its child animations.

### Properties / fields

| Member | Description |
|---|---|
| `Children` | Gets the collection of child `Timeline` objects. <br>Returns: The collection of child `Timeline` objects. The default is an empty collection. |
| `TargetName` | Gets or sets the name of the object to animate. <br>Returns: The name of the object to animate. |
| `TargetProperty` | Gets or sets the name of the property that should be animated. <br>Returns: The property to animate. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `TargetNameProperty` | Identifies the `TargetName` attached property. <br>Returns: The identifier for the `TargetName` attached property. |
| `TargetPropertyProperty` | Identifies the `TargetProperty` attached property. <br>Returns: The identifier for the `TargetProperty` attached property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Storyboard` class. |
| `Begin` | Initiates the set of animations associated with the storyboard. |
| `GetCurrentState` | Gets the clock state of the storyboard. <br>Returns: One of the enumeration values: `Active`, `Filling`, or `Stopped`. |
| `GetCurrentTime` | Gets the current time of the storyboard. <br>Returns: The current time of the storyboard, or null if the storyboard's clock is `Stopped`. |
| `GetTargetName(Media.Animation.Timeline)` | Gets the `TargetName` of the specified `Timeline` object. <br>`element`: The `Timeline` object to get the target name from. <br>Returns: The string name of the target object. |
| `GetTargetProperty(Media.Animation.Timeline)` | Gets the `TargetProperty` of the specified `Timeline` object. <br>`element`: The `Timeline` object to get the target property from. <br>Returns: The property path information for the animated property. |
| `Pause` | Pauses the animation clock associated with the storyboard. |
| `Resume` | Resumes the animation clock, or run-time state, associated with the storyboard. |
| `Seek(TimeSpan)` | Moves the storyboard to the specified animation position. The storyboard performs the requested seek when the next clock tick occurs. <br>`offset`: A positive or negative time value that describes the amount by which the timeline should move forward or backward from the beginning of the animation. By using the `TimeSpan` Parse behavior, a `TimeSpan` can be specified as a string in the following format (in this syntax, the [] characters denote optional components of the string, but the quotes, colons, and periods are all a literal part of the syntax):"[days.]hours:minutes:seconds[.fractionalSeconds]"- or -"days" |
| `SeekAlignedToLastTick(TimeSpan)` | Moves the storyboard to the specified animation position immediately (synchronously). <br>`offset`: A positive or negative time value that describes the amount by which the timeline should move forward or backward from the beginning of the animation. By using the `TimeSpan` Parse behavior, a `TimeSpan` can be specified as a string in the following format (in this syntax, the [] characters denote optional components of the string, but the quotes, colons, and periods are all a literal part of the syntax):"[days.]hours:minutes:seconds[.fractionalSeconds]"- or -"days" |
| `SetTarget(Media.Animation.Timeline,DependencyObject)` | Causes the specified `Timeline` to target the specified object. <br>`timeline`: The timeline that targets the specified dependency object. <br>`target`: The actual instance of the object to target. |
| `SetTargetName(Media.Animation.Timeline,String)` | Causes the specified `Timeline` to target the object with the specified name. <br>`element`: The timeline that targets the specified dependency object. <br>`name`: The name of the object to target. |
| `SetTargetProperty(Media.Animation.Timeline,PropertyPath)` | Causes the specified `Timeline` to target the specified dependency property. <br>`element`: The timeline with which to associate the specified dependency property. <br>`path`: A path that describe the dependency property to be animated. |
| `SkipToFill` | Advances the current time of the storyboard's clock to the end of its active period. |
| `Stop` | Stops the storyboard. |

## System.Windows.Media.Animation.Timeline

Defines a segment of time.

### Properties / fields

| Member | Description |
|---|---|
| `AutoReverse` | Gets or sets a value that indicates whether the timeline plays in reverse after it completes a forward iteration. <br>Returns: true if the timeline plays in reverse at the end of each iteration; otherwise, false. The default value is false. |
| `BeginTime` | Gets or sets the time at which this `Timeline` should begin. <br>Returns: The start time of the time line. The default value is zero. |
| `Duration` | Gets or sets the length of time for which this timeline plays, not counting repetitions. <br>Returns: The timeline's simple duration: the amount of time this timeline takes to complete a single forward iteration. The default value is `Automatic`. |
| `FillBehavior` | Gets or sets a value that specifies how the animation behaves after it reaches the end of its active period. <br>Returns: A value that specifies how the timeline behaves after it reaches the end of its active period but its parent is inside its active or fill period. The default value is `HoldEnd`. |
| `RepeatBehavior` | Gets or sets the repeating behavior of this timeline. <br>Returns: An iteration `Count` that specifies the number of times the timeline should play, a `TimeSpan` value that specifies the total length of this timeline's active period, or the special value `Forever`, which specifies that the timeline should repeat indefinitely. The default value is a `RepeatBehavior` with a `Count` of 1, which indicates that the timeline plays once. |
| `SpeedRatio` | Gets or sets the rate, relative to its parent, at which time progresses for this `Timeline`. <br>Returns: A finite value greater than 0 that specifies the rate at which time progresses for this timeline, relative to the speed of the timeline's parent. If this timeline is a root timeline, specifies the default timeline speed. The value is expressed as a factor where 1 represents normal speed, 2 is double speed, 0.5 is half speed, and so on. The default value is 1. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AutoReverseProperty` | Identifies the `AutoReverse` dependency property. <br>Returns: The identifier for the `AutoReverse` dependency property. |
| `BeginTimeProperty` | Identifies the `BeginTime` dependency property. <br>Returns: The identifier for the `BeginTime` dependency property. |
| `DurationProperty` | Identifies the `Duration` dependency property. <br>Returns: The identifier for the `Duration` dependency property. |
| `FillBehaviorProperty` | Identifies the `FillBehavior` dependency property. <br>Returns: The identifier for the `FillBehavior` dependency property. |
| `RepeatBehaviorProperty` | Identifies the `RepeatBehavior` dependency property. <br>Returns: The identifier for the `RepeatBehavior` dependency property. |
| `SpeedRatioProperty` | Identifies for the `SpeedRatio` dependency property. <br>Returns: The identifier for the `SpeedRatio` dependency property. |

### Events

| Member | Description |
|---|---|
| `Completed` | Occurs when the `Storyboard` object has completed playing. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Timeline` class. |

## System.Windows.Media.Animation.TimelineCollection

Represents a collection of `Timeline` objects.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TimelineCollection` class. |

## System.Windows.Media.ArcSegment

Represents an elliptical arc between two points.

### Properties / fields

| Member | Description |
|---|---|
| `IsLargeArc` | Gets or sets a value that indicates whether the arc should be greater than 180 degrees. <br>Returns: true if the arc should be greater than 180 degrees; otherwise, false. The default is false. |
| `Point` | Gets or sets the endpoint of the elliptical arc. <br>Returns: The point to which the arc is drawn. The default is a `Point` with value 0,0. |
| `RotationAngle` | Gets or sets the amount (in degrees) by which the ellipse is rotated about the x-axis. <br>Returns: The amount (in degrees) by which the ellipse is rotated about the x-axis. The default is 0. |
| `Size` | Gets or sets the x- and y-radius of the arc as a `Size` structure. <br>Returns: A `Size` structure that describes the x- and y-radius of the elliptical arc. The `Size` structure's `Width` property specifies the arc's x-radius; its `Height` property specifies the arc's y-radius. The default is a `Size` with value 0,0. |
| `SweepDirection` | Gets or sets a value that specifies whether the arc is drawn in the `Clockwise` or `Counterclockwise` direction. <br>Returns: One of the enumeration values that specifies the direction in which the arc is drawn. The default is `Counterclockwise`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsLargeArcProperty` | Identifies the `IsLargeArc` dependency property. <br>Returns: The `IsLargeArc` dependency property identifier. |
| `PointProperty` | Identifies the `Point` dependency property. <br>Returns: The `Point` dependency property identifier. |
| `RotationAngleProperty` | Identifies the `RotationAngle` dependency property. <br>Returns: The `RotationAngle` dependency property identifier. |
| `SizeProperty` | Identifies the `Size` dependency property. <br>Returns: The `Size` dependency property identifier. |
| `SweepDirectionProperty` | Identifies the `SweepDirection` dependency property. <br>Returns: The `SweepDirection` dependency property identifier. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ArcSegment` class. |

## System.Windows.Media.AudioCaptureDevice

Describes either the desired or supported formats for an audio capture device, such as a microphone.

### Properties / fields

| Member | Description |
|---|---|
| `AudioFrameSize` | Gets or sets the intended audio frame size. <br>Returns: The audio frame size, in milliseconds. The default is 1000. See Remarks. |
| `DesiredFormat` | Gets or sets the desired audio format to be used by this audio capture device. <br>Returns: The desired audio format, as an `AudioFormat` object, that is used by this audio capture device. The default value is null. |
| `SupportedFormats` | Gets a generic collection of supported audio formats for this audio capture device. <br>Returns: A generic collection of `AudioFormat` objects that represents the supported audio formats for this audio capture device. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AudioFrameSizeProperty` | Identifies the `AudioFrameSize` dependency property. <br>Returns: The identifier for the `AudioFrameSize` dependency property. |

## System.Windows.Media.AudioCaptureDeviceCollection

Specifies a collection of `AudioCaptureDevice` objects.

## System.Windows.Media.AudioFormat

Provides specific technical information about an audio format that is used by audio capture devices.

### Properties / fields

| Member | Description |
|---|---|
| `BitsPerSample` | Gets the number of bits that are used to store the audio information for a single sample of an audio format. <br>Returns: The number of bits that are used to store the audio information for a single sample of an audio format. |
| `Channels` | Gets the number of channels that are provided by the audio format. <br>Returns: The number of channels that are provided by the audio format. |
| `SamplesPerSecond` | Gets the number of samples per second that are provided by the audio format. <br>Returns: The number of samples per second that are provided by the audio format. |
| `WaveFormat` | Gets the encoding format of the audio format as a `WaveFormatType` value. <br>Returns: The encoding format of the audio format. |

## System.Windows.Media.AudioSink

Exposes the capture graph for audio devices. Derive from this class to receive audio information and to obtain the capture graph through `CaptureSource`.

### Properties / fields

| Member | Description |
|---|---|
| `CaptureSource` | Gets or sets a capture source that is associated with this `AudioSink`. <br>Returns: A capture source that is associated with this `AudioSink`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AudioSink` class. |
| `Finalize` | Frees the resources that are used by the `AudioSink` class. |
| `OnCaptureStarted` | When overridden in a derived class, is invoked when an audio device starts capturing audio data. |
| `OnCaptureStopped` | When overridden in a derived class, is invoked when an audio device stops capturing audio data. |
| `OnFormatChange(Media.AudioFormat)` | When overridden in a derived class, is invoked when an audio device reports an audio format change. <br>`audioFormat`: The new audio format. |
| `OnSamples(Int64,Int64,Byte[])` | When overridden in a derived class, is invoked when an audio device captures a complete audio sample. <br>`sampleTimeInHundredNanoseconds`: The time, in 100-nanosecond units, when the sample was captured. <br>`sampleDurationInHundredNanoseconds`: The duration of the sample, in 100-nanosecond units. <br>`sampleData`: A byte stream that contains audio data, to be interpreted per the relevant audio format information. |

## System.Windows.Media.BezierSegment

Represents a cubic Bezier curve drawn between two points.

### Properties / fields

| Member | Description |
|---|---|
| `Point1` | Gets or sets the first control point of the curve. <br>Returns: The first control point of the curve. The default is a `Point` with value 0,0. |
| `Point2` | Gets or sets the second control point of the curve. <br>Returns: The second control point of the curve. |
| `Point3` | Gets or sets the end point of the curve. <br>Returns: The end point of the curve. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Point1Property` | Identifies the `Point1` dependency property. <br>Returns: The identifier for the `Point1` dependency property. |
| `Point2Property` | Identifies the `Point2` dependency property. <br>Returns: The identifier for the `Point2` dependency property. |
| `Point3Property` | Identifies the `Point3` dependency property. <br>Returns: The identifier for the `Point3` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BezierSegment` class. |

## System.Windows.Media.BitmapCache

Represents the behavior of caching a visual element or tree of elements as bitmap surfaces. This can yield significant performance improvements for some scenarios.

### Properties / fields

| Member | Description |
|---|---|
| `RenderAtScale` | Gets or sets the scale at which the object is rendered on the cached bitmap surface. Use this property for cached objects that are scaled to improve performance. <br>Returns: The scale at which the object is rendered as a cached bitmap. If you specify a negative number, an error will be thrown. The default is 1. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `RenderAtScaleProperty` | Identifies the `RenderAtScale` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BitmapCache` class. |

## System.Windows.Media.Brush

Defines objects used to paint graphical objects. Classes that derive from `Brush` describe how the area is painted.

### Properties / fields

| Member | Description |
|---|---|
| `Opacity` | Gets or sets the degree of opacity of a `Brush`. <br>Returns: The value of the `Opacity` property is expressed as a value between 0 and 1.0. The default value is 1.0. |
| `RelativeTransform` | Gets or sets the transformation that is applied to the brush using relative coordinates. <br>Returns: The transformation that is applied to the brush using relative coordinates. The default value is null. |
| `Transform` | Gets or sets the transformation that is applied to the brush. <br>Returns: The transformation to apply to the brush. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `OpacityProperty` | Identifies the `Opacity` dependency property. <br>Returns: The `Opacity` dependency property identifier. |
| `RelativeTransformProperty` | Identifies the `RelativeTransform` dependency property. <br>Returns: The `RelativeTransform` dependency property identifier. |
| `TransformProperty` | Identifies the `Transform` dependency property. <br>Returns: The `Transform` dependency property identifier. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Brush` class. |

## System.Windows.Media.BrushMappingMode

Specifies the coordinate system used by a `Brush`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Absolute` | The coordinate system is not relative to a bounding box. Values are interpreted directly in local space. |
| `RelativeToBoundingBox` | The coordinate system is relative to a bounding box: 0 indicates 0 percent of the bounding box, and 1 indicates 100 percent of the bounding box. For example, (0.5, 0.5) describes a point in the middle of the bounding box, and (1, 1) describes a point at the bottom right of the bounding box. |

## System.Windows.Media.CacheMode

Represents cached content.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CacheMode` class. |

## System.Windows.Media.CaptureDevice

Provides common properties for `AudioCaptureDevice` and `VideoCaptureDevice`.

### Properties / fields

| Member | Description |
|---|---|
| `FriendlyName` | Gets the UI display name for the capture device. <br>Returns: The UI display name for the capture device. |
| `IsDefaultDevice` | Gets a value that declares whether the capture device is the default device for its type (audio or video). <br>Returns: true if this `CaptureDevice` represents the default device for its type; otherwise, false. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FriendlyNameProperty` | Identifies the `FriendlyName` dependency property. <br>Returns: The identifier for the `FriendlyName` dependency property. |
| `IsDefaultDeviceProperty` | Identifies the `IsDefaultDevice` dependency property. <br>Returns: The identifier for the `IsDefaultDevice` dependency property. |

## System.Windows.Media.CaptureDeviceConfiguration

Represents a helper class for obtaining information about available capture devices (audio or video) and requesting client user permission to access the captures from available devices.

### Properties / fields

| Member | Description |
|---|---|
| `AllowedDeviceAccess` | Gets a value that reports whether a user has previously granted device access based on their stored response to the device access UI prompt. <br>Returns: true if the user has previously granted device access based on their response to the access request dialog box prompt, or if the request is automatically granted in some circumstances. false if the user does not grant access or if the request is denied by other factors. See Remarks. |

### Methods

| Member | Description |
|---|---|
| `GetAvailableAudioCaptureDevices` | Returns a collection of `AudioCaptureDevice` objects that represent the available audio capture devices on the client system. <br>Returns: A generic read-only collection of `AudioCaptureDevice` objects that represent the available audio capture devices on the client system. |
| `GetAvailableVideoCaptureDevices` | Returns a collection of `VideoCaptureDevice` objects that represent the available video capture devices on the client system. <br>Returns: A generic read-only collection of `VideoCaptureDevice` objects that represent the available video capture devices on the client system. |
| `GetDefaultAudioCaptureDevice` | Returns the `AudioCaptureDevice` object that represent the default audio capture devices on the client system. <br>Returns: The `AudioCaptureDevice` object that represent the default audio capture devices on the client system.On Mac OSX, if an application calls `GetDefaultAudioCaptureDevice` or `GetDefaultVideoCaptureDevice` and the default audio and video devices have not previously been set by the user, Silverlight will return the first device found. If a physical device is not connected to this `CaptureDevice`, an `InvalidOperationException` will be thrown when `Start` is called. This expectation should be caught by the application and the user can then be notified to manually set their default devices in the "Webcam/Mic" section of the Configuration Settings dialog. |
| `GetDefaultVideoCaptureDevice` | Returns the `VideoCaptureDevice` object that represent the default video capture devices on the client system. <br>Returns: The `VideoCaptureDevice` object that represent the default video capture devices on the client system.On Mac OSX, if an application calls `GetDefaultAudioCaptureDevice` or `GetDefaultVideoCaptureDevice` and the default audio and video devices have not previously been set by the user, Silverlight will return the first device found. If a physical device is not connected to this `CaptureDevice`, an `InvalidOperationException` will be thrown when `Start` is called. This expectation should be caught by the application and the user can then be notified to manually set their default devices in the "Webcam/Mic" section of the Configuration Settings dialog. |
| `RequestDeviceAccess` | Requests access for all possible capture devices (audio or video) that are available on the client system. <br>Returns: true if the user grants device access based on their response to the access request dialog prompt, or if the request is automatically granted in some circumstances. false if the user does not grant access or if the request is denied by other factors. See Remarks. |

## System.Windows.Media.CaptureImageCompletedEventArgs

Provides event data for the `CaptureImageCompleted` event of the `CaptureSource` class.

### Properties / fields

| Member | Description |
|---|---|
| `Result` | Gets the `WriteableBitmap` image source to report in event data. <br>Returns: The image source that is returned asynchronously through this event data, as a result of a `CaptureImageAsync` call. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Media.Imaging.WriteableBitmap)` | Initializes a new instance of the `CaptureImageCompletedEventArgs` using an image source to report in event data. <br>`image`: The image source to report. |

## System.Windows.Media.CaptureSource

Provides methods that work with specific audio or video captures from the associated capture device.

### Properties / fields

| Member | Description |
|---|---|
| `AudioCaptureDevice` | Gets or sets the associated audio capture device that is relevant to this capture. <br>Returns: The associated audio capture device that is relevant to this capture. |
| `State` | Gets the `CaptureState` of this capture. State might indicate that the capture is still ongoing or has failed. <br>Returns: The `CaptureState` of this capture. |
| `VideoCaptureDevice` | Gets or sets the associated video capture device that is relevant to this capture. <br>Returns: The associated video capture device that is relevant to this capture. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AudioCaptureDeviceProperty` | Identifies the `AudioCaptureDevice` dependency property. <br>Returns: The identifier for the `AudioCaptureDevice` dependency property. |
| `VideoCaptureDeviceProperty` | Identifies the `VideoCaptureDevice` dependency property. <br>Returns: The identifier for the `VideoCaptureDevice` dependency property. |

### Events

| Member | Description |
|---|---|
| `CaptureFailed` | Occurs when a requested capture operation does not generate a captured sample. |
| `CaptureImageCompleted` | Occurs when an asynchronous image capture request has returned a captured image. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CaptureSource` class. |
| `CaptureImageAsync` | Initiates an asynchronous image capture request. Retrieve the returned image by handling the `CaptureImageCompleted` event on this `CaptureSource`. |
| `Start` | Initiates capture from all capture devices that are relevant to this `CaptureSource`. |
| `Stop` | Stops capture from all capture devices that are relevant to this `CaptureSource`. |

## System.Windows.Media.CaptureState

Describes the current operation state for device capture and a capture source.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Failed` | The capture failed, or no operation has yet been attempted. |
| `Started` | `Start` has been called and the capture is still active. |
| `Stopped` | `Stop` has been called and the capture is complete. |

## System.Windows.Media.Color

Describes a color in terms of alpha, red, green, and blue channels.

### Properties / fields

| Member | Description |
|---|---|
| `A` | Gets or sets the sRGB alpha channel value of the color. <br>Returns: The sRGB alpha channel value of the color, as a value between 0 and 255. |
| `B` | Gets or sets the sRGB blue channel value of the color. <br>Returns: The sRGB blue channel value, as a value between 0 and 255. |
| `G` | Gets or sets the sRGB green channel value of the color. <br>Returns: The sRGB green channel value, as a value between 0 and 255. |
| `R` | Gets or sets the sRGB red channel value of the color. <br>Returns: The sRGB red channel value, as a value between 0 and 255. |

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Tests whether the specified object is a `Color` structure and is equivalent to the current color. <br>`o`: The object to compare to the current `Color` structure. <br>Returns: true if the specified object is a `Color` structure and is identical to the current `Color` structure; otherwise, false. |
| `Equals(Media.Color)` | Tests whether the specified `Color` structure is identical to the current color. <br>`color`: The `Color` structure to compare to the current `Color` structure. <br>Returns: true if the specified `Color` structure is identical to the current `Color` structure; otherwise, false. |
| `FromArgb(Byte,Byte,Byte,Byte)` | Creates a new `Color` structure by using the specified sRGB alpha channel and color channel values. <br>`a`: The alpha channel, `A`, of the new color. The value must be between 0 and 255. <br>`r`: The red channel, `R`, of the new color. The value must be between 0 and 255. <br>`g`: The green channel, `G`, of the new color. The value must be between 0 and 255. <br>`b`: The blue channel, `B`, of the new color. The value must be between 0 and 255. <br>Returns: A `Color` structure with the specified values. |
| `GetHashCode` | Gets a hash code for the current `Color` structure. <br>Returns: A hash code for the current `Color` structure. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `IFormatProvider)`. <br>`format`: The string specifying the format to use. -or- null to use the default format defined for the type of the IFormattable implementation. <br>`provider`: The IFormatProvider to use to format the value. -or- null to obtain the numeric format information from the current locale setting of the operating system. <br>Returns: A string containing the value of the current instance in the specified format. |
| `ToString` | Creates a string representation of the color using the ARGB channels in hex notation. <br>Returns: The string representation of the color. |
| `ToString(IFormatProvider)` | Creates a string representation of the color by using the ARGB channels and the specified format provider. <br>`provider`: Culture-specific formatting information. <br>Returns: The string representation of the color. |
| `op_Equality(Media.Color,Media.Color)` | Tests whether two `Color` structures are identical. <br>`color1`: The first `Color` structure to compare. <br>`color2`: The second `Color` structure to compare. <br>Returns: true if and are exactly identical; otherwise, false. |
| `op_Inequality(Media.Color,Media.Color)` | Tests whether two `Color` structures are not identical. <br>`color1`: The first `Color` structure to compare. <br>`color2`: The second `Color` structure to compare. <br>Returns: true if and are not equal; otherwise, false. |

## System.Windows.Media.ColorInterpolationMode

Determines how the colors in a gradient are interpolated.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SRgbLinearInterpolation` | Colors are interpolated in the sRGB color space |
| `ScRgbLinearInterpolation` | Colors are interpolated in the scRGB color space |

## System.Windows.Media.Colors

Implements a set of predefined colors.

### Properties / fields

| Member | Description |
|---|---|
| `Black` | Gets the system-defined color that has the ARGB value of #FF000000. <br>Returns: A `Color` that has an ARGB value of #FF000000. |
| `Blue` | Gets the system-defined color that has the ARGB value of #FF0000FF. <br>Returns: A `Color` that has the ARGB value of #FF0000FF. |
| `Brown` | Gets the system-defined color that has the ARGB value of #FFA52A2A. <br>Returns: A `Color` that has the ARGB value of #FFA52A2A. |
| `Cyan` | Gets the system-defined color that has the ARGB value of #FF00FFFF. <br>Returns: A `Color` that has the ARGB value of #FF00FFFF. |
| `DarkGray` | Gets the system-defined color that has the ARGB value of #FFA9A9A9. <br>Returns: A `Color` that has the ARGB value of #FFA9A9A9. |
| `Gray` | Gets the system-defined color that has the ARGB value of #FF808080. <br>Returns: A `Color` that has the ARGB value of #FF808080. |
| `Green` | Gets the system-defined color that has the ARGB value of #FF008000. <br>Returns: A `Color` that has the ARGB value of #FF008000. |
| `LightGray` | Gets the system-defined color that has the ARGB value of #FFD3D3D3. <br>Returns: A `Color` that has the ARGB value of #FFD3D3D3. |
| `Magenta` | Gets the system-defined color that has the ARGB value of #FFFF00FF. <br>Returns: A `Color` that has the ARGB value of #FFFF00FF. |
| `Orange` | Gets the system-defined color that has the ARGB value of #FFFFA500. <br>Returns: A `Color` that has the ARGB value of #FFFFA500. |
| `Purple` | Gets the system-defined color that has the ARGB value of #FF800080. <br>Returns: A `Color` that has the ARGB value of #FF800080. |
| `Red` | Gets the system-defined color that has the ARGB value of #FFFF0000. <br>Returns: A `Color` that has the ARGB value of #FFFF0000. |
| `Transparent` | Gets the system-defined color that has the ARGB value of #00FFFFFF. <br>Returns: A `Color` that has the ARGB value of #00FFFFFF. |
| `White` | Gets the system-defined color that has the ARGB value of #FFFFFFFF. <br>Returns: A `Color` that has the ARGB value of #FFFFFFFF. |
| `Yellow` | Gets the system-defined color that has the ARGB value of #FFFFFF00. <br>Returns: A `Color` that has the ARGB value of #FFFFFF00. |

## System.Windows.Media.CompositeTransform

This class lets you apply multiple different transforms to an object.

### Properties / fields

| Member | Description |
|---|---|
| `CenterX` | Gets or sets the x-coordinate of the center point for all transforms specified by the `CompositeTransform`. <br>Returns: The x-coordinate of the center point for all transforms specified by the `CompositeTransform`. |
| `CenterY` | Gets or sets the y-coordinate of the center point for all transforms specified by the `CompositeTransform`. <br>Returns: The y-coordinate of the center point for all transforms specified by the `CompositeTransform`. |
| `Rotation` | Gets or sets the angle, in degrees, of clockwise rotation. <br>Returns: The angle, in degrees, of clockwise rotation. The default is 0. |
| `ScaleX` | Gets or sets the x-axis scale factor. You can use this property to stretch or shrink an object horizontally. <br>Returns: The scale factor along the x-axis. The default is 1. |
| `ScaleY` | Gets or sets the y-axis scale factor. You can use this property to stretch or shrink an object vertically. <br>Returns: The scale factor along the y-axis. The default is 1. |
| `SkewX` | Gets or sets the x-axis skew angle, which is measured in degrees counterclockwise from the y-axis. A skew transform can be useful for creating the illusion of three-dimensional depth in a two-dimensional object. <br>Returns: The skew angle, which is measured in degrees counterclockwise from the y-axis. The default is 0. |
| `SkewY` | Gets or sets the y-axis skew angle, which is measured in degrees counterclockwise from the x-axis. A skew transform can be useful for creating the illusion of three-dimensional depth in a two-dimensional object. <br>Returns: The skew angle, which is measured in degrees counterclockwise from the x-axis. The default is 0. |
| `TranslateX` | Gets or sets the distance to translate along the x-axis. <br>Returns: The distance to translate (move) an object along the x-axis, in pixels. This property is read/write. The default is 0. |
| `TranslateY` | Gets or sets the distance to translate (move) an object along the y-axis. <br>Returns: The distance to translate (move) an object along the y-axis, in pixels. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterXProperty` | Identifies the `CenterX` dependency property. <br>Returns: The identifier for the `CenterX` dependency property. |
| `CenterYProperty` | Identifies the `CenterY` dependency property. <br>Returns: The identifier for the `CenterY` dependency property. |
| `RotationProperty` | Identifies the `Rotation` dependency property. <br>Returns: The identifier for the `Rotation` dependency property. |
| `ScaleXProperty` | Identifies the `ScaleX` dependency property. <br>Returns: The identifier for the `ScaleX` dependency property. |
| `ScaleYProperty` | Identifies the `ScaleY` dependency property. <br>Returns: The identifier for the `ScaleY` dependency property. |
| `SkewXProperty` | Identifies the `SkewX` dependency property. <br>Returns: The identifier for the `SkewX` dependency property. |
| `SkewYProperty` | Identifies the `SkewY` dependency property. <br>Returns: The identifier for the `SkewY` dependency property. |
| `TranslateXProperty` | Identifies the `TranslateX` dependency property. <br>Returns: The identifier for the `TranslateX` dependency property. |
| `TranslateYProperty` | Identifies the `TranslateY` dependency property. <br>Returns: The identifier for the `TranslateY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CompositeTransform` class. |

## System.Windows.Media.CompositionTarget

Represents the display surface of a Silverlight-based application.

### Events

| Member | Description |
|---|---|
| `Rendering` | Occurs when the core Silverlight rendering process renders a frame. |

## System.Windows.Media.ContentKeyType

Represents the content key type.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Aes128Bit` | Content key type used for Root licenses (AES-ECB). |
| `Cocktail` | Content key type used for Cocktail in the PlayReady ecosystem. |

## System.Windows.Media.DeepZoomImageTileSource

Used to specify the source of a `MultiScaleImage`.

### Properties / fields

| Member | Description |
|---|---|
| `UriSource` | Gets or sets the source Uri of the `DeepZoomImageTileSource`. <br>Returns: The source Uri of the `DeepZoomImageTileSource`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `UriSourceProperty` | Identifies the `UriSource` dependency property. <br>Returns: The identifier for the `UriSource` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `DeepZoomImageTileSource` class. |
| `Constructor(Uri)` | Creates a new instance of the `DeepZoomImageTileSource` class with the specified source Uri. <br>`sourceUri`: The source Uri. |

## System.Windows.Media.DomainAcquirer

Handles Join Domain requests.

### Properties / fields

| Member | Description |
|---|---|
| `ChallengeCustomData` | Gets or sets a string containing service specific data to be conveyed to the domain server without implementing manual domain join/leave. <br>Returns: A string containing service specific data to be conveyed to the domain server without implementing manual domain join/leave. The default value is null. |

### Events

| Member | Description |
|---|---|
| `JoinDomainCompleted` | Occurs when the join domain operation completes. |
| `LeaveDomainCompleted` | Occurs when the leave domain operation completes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DomainAcquirer` class. |
| `CancelAsync` | Called to cancel an ongoing domain operation. |
| `JoinDomainAsync(Guid,Guid,Uri,String)` | Starts an asynchronous join domain operation. <br>`serviceId`: The service identifier for this domain. <br>`accountId`: The account identifier for this domain. <br>`domainServerUrl`: URL to the domain controller server which is used to manage domains. <br>`friendlyName`: The friendly name for the account that can be shown to the user during domain certificate enumeration on the client application. |
| `LeaveDomainAsync(Guid,Guid,Uri)` | Method used to start an asynchronous leave domain operation. <br>`serviceId`: The service identifier for this domain. <br>`accountId`: The account identifier for this domain. <br>`domainServerUrl`: URL to the domain controller server which is used to manage domains. |
| `OnCancel` | Overridden in a derived class to implement cancellation during manual domain operations. |
| `OnJoinDomain(IO.Stream,Uri)` | Overridden in a derived class to implement manual join domain operations. <br>`joinChallenge`: The custom authentication information you wish to add to the join domain operation. <br>`domainServerUri`: URI to the domain controller server which is used to manage domains. |
| `OnLeaveDomain(IO.Stream,Uri)` | Overridden in a derived class to implement manual leave domain operations. <br>`leaveChallenge`: The custom authentication information you wish to add to the leave domain operation. <br>`domainServerUri`: URI to the domain controller server which is used to manage domains. |
| `SetJoinDomainResponse(IO.Stream)` | Used to set the join domain operation response during a manual join domain operation in a derived `DomainAcquirer`. <br>`domainResponse`: The join domain operation response sent back to the client by the domain controller server. |
| `SetLeaveDomainResponse(IO.Stream)` | Used to set the Leave domain operation response during a manual leave domain operation in a derived `DomainAcquirer`. <br>`domainResponse`: The leave domain operation response sent back to the client by the domain controller server. |

## System.Windows.Media.DomainOperationCompletedEventArgs

Provides data for join and leave domain events.

### Properties / fields

| Member | Description |
|---|---|
| `AccountId` | Gets the account identifier for this domain. <br>Returns: The account identifier for this domain. |
| `ResponseCustomData` | Gets the custom data string that is returned from the domain operation. <br>Returns: The custom data string that is returned from the domain operation. |
| `ServiceId` | Gets the service identifier for this domain. <br>Returns: The service identifier for this domain. |

## System.Windows.Media.DoubleCollection

Represents an ordered collection of `Double` values.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DoubleCollection` class. |

## System.Windows.Media.Effects.BlurEffect

Represents an effect that you can apply to an object that simulates looking at the object through an out-of-focus lens.

### Properties / fields

| Member | Description |
|---|---|
| `Radius` | Gets or sets the amount of blurring applied by the `BlurEffect`. <br>Returns: The radius used in the blur, as a pixel-based factor. The default is 5. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `RadiusProperty` | Identifies the `Radius` dependency property. <br>Returns: The identifier for the `Radius` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BlurEffect` class. |

## System.Windows.Media.Effects.DropShadowEffect

Applies a shadow behind a visual object at a slight offset. The offset is determined by mimicking a casting shadow from an imaginary light source.

### Properties / fields

| Member | Description |
|---|---|
| `BlurRadius` | Gets or sets how defined the edges of the shadow are (how blurry the shadow is). <br>Returns: How blurry the shadow is. The default is 5. |
| `Color` | Gets or sets the color of the shadow. <br>Returns: The color of the shadow. The default is a color with ARGB value of FF000000 (black). |
| `Direction` | Gets or sets the angle at which the shadow is cast. <br>Returns: The angle at which the shadow is cast, where 0 is immediately to the right of the object and positive values move the shadow counterclockwise. The default is 315. |
| `Opacity` | Gets or sets the degree of opacity of the shadow. <br>Returns: The degree of opacity. The valid range of values is from 0 through 1, where 0 is completely transparent and 1 is completely opaque. The default is 1. |
| `ShadowDepth` | Gets or sets the distance between the object and the shadow that it casts. <br>Returns: The distance between the plane of the object casting the shadow and the shadow plane measured in devicepixels. The valid range of values is from 0 through 300. The default is 5. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BlurRadiusProperty` | Identifies the `BlurRadius` dependency property. <br>Returns: The identifier for the `BlurRadius` dependency property. |
| `ColorProperty` | Identifies the `Color` dependency property. <br>Returns: The identifier for the `Color` dependency property. |
| `DirectionProperty` | Identifies the `Direction` dependency property. <br>Returns: The identifier for the `Direction` dependency property. |
| `OpacityProperty` | Identifies the `Opacity` dependency property. <br>Returns: The identifier for the `Opacity` dependency property. |
| `ShadowDepthProperty` | Identifies the `ShadowDepth` dependency property. <br>Returns: The identifier for the `ShadowDepth` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DropShadowEffect` class. |

## System.Windows.Media.Effects.Effect

Provides a base class for all bitmap effects.

### Properties / fields

| Member | Description |
|---|---|
| `EffectMapping` | When overridden in a derived class, transforms mouse input and coordinate systems through the effect. <br>Returns: The transform to apply. The default is the identity transform. |
| `ImplicitInput` | Gets a `Brush` that, when it is used as an input for an `Effect`, causes the bitmap of the `UIElement` that the `Effect` is applied to be that input. <br>Returns: The `Brush` that acts as the input. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Effect` class. |

## System.Windows.Media.Effects.PixelShader

Provides a managed wrapper around a High Level Shading Language (HLSL) pixel shader.

### Properties / fields

| Member | Description |
|---|---|
| `UriSource` | Gets or sets a URI reference to HLSL bytecode in the assembly. <br>Returns: The URI reference to HLSL bytecode in the assembly. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `UriSourceProperty` | Identifies the `UriSource` dependency property. <br>Returns: The identifier for the `UriSource` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PixelShader` class. |

## System.Windows.Media.Effects.SamplingMode

Specifies how dependency properties with `Brush` values are sampled in a custom shader effect.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Auto` | The system selects the most appropriate sampling mode. |
| `Bilinear` | Use bilinear sampling. |
| `NearestNeighbor` | Use nearest neighbor sampling. |

## System.Windows.Media.Effects.ShaderEffect

Provides a custom bitmap effect by using a `PixelShader`.

### Properties / fields

| Member | Description |
|---|---|
| `DdxUvDdyUvRegisterIndex` | Gets or sets a value that indicates the shader register to use for the partial derivatives of the texture coordinates with respect to screen space. <br>Returns: The index of the register that contains the partial derivatives. |
| `PaddingBottom` | Gets or sets the amount by which the effect's output texture is larger than its input texture along the bottom edge of the effect. <br>Returns: The padding along the bottom edge of the effect. |
| `PaddingLeft` | Gets or sets the amount by which the effect's output texture is larger than its input texture along the left edge. <br>Returns: The padding along the left edge of the effect. |
| `PaddingRight` | Gets or sets the amount by which the effect's output texture is larger than its input texture along the right edge. <br>Returns: The padding along the right edge of the effect. |
| `PaddingTop` | Gets or sets the amount by which the effect's output texture is larger than its input texture along the top edge. <br>Returns: The padding along the top edge of the effect. |
| `PixelShader` | Gets or sets the `PixelShader` to use for the effect. <br>Returns: The `PixelShader` for the effect. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PixelShaderProperty` | Identifies the `PixelShader` dependency property. <br>Returns: The identifier for the `PixelShader` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ShaderEffect` class. |
| `PixelShaderConstantCallback(Int32)` | Associates a dependency property value with a pixel shader's float constant register. <br>`register`: The index of the shader register associated with the dependency property. <br>Returns: A `PropertyChangedCallback` delegate that associates a dependency property and the shader constant register specified by `register`. |
| `PixelShaderSamplerCallback(Int32)` | Associates a dependency property value with a pixel shader's sampler register. <br>`register`: The index of the shader sampler associated with the dependency property. <br>Returns: A `PropertyChangedCallback` delegate that associates a dependency property and the shader sampler register specified by `register`. |
| `PixelShaderSamplerCallback(Int32,Media.Effects.SamplingMode)` | Associates a dependency property value with a pixel shader's sampler register and a `SamplingMode`. <br>`register`: The index of the shader sampler associated with the dependency property. <br>`samplingMode`: The `SamplingMode` for the shader sampler. <br>Returns: A `PropertyChangedCallback` delegate that associates a dependency property and the shader sampler register specified by `register`. |
| `RegisterPixelShaderSamplerProperty(String,Type,Int32)` | Associates a dependency property with a shader sampler register. <br>`dpName`: The name of the dependency property. <br>`ownerType`: The type of the effect that has the dependency property. <br>`samplerRegisterIndex`: The index of the shader sampler associated with the dependency property. <br>Returns: A dependency property associated with the shader sampler specified by . |
| `RegisterPixelShaderSamplerProperty(String,Type,Int32,Media.Effects.SamplingMode)` | Associates a dependency property with a shader sampler register and a sampling mode. <br>`dpName`: The name of the dependency property. <br>`ownerType`: The type of the effect that has the dependency property. <br>`samplerRegisterIndex`: The index of the shader sampler associated with the dependency property. <br>`samplingMode`: One of the enumeration values that specifies the sampling mode for the shader. <br>Returns: A dependency property associated with the shader sampler specified by . |
| `UpdateShaderValue(DependencyProperty)` | Notifies the effect that the shader constant or sampler corresponding to the specified dependency property should be updated. <br>`dp`: The dependency property to be updated. |

## System.Windows.Media.EllipseGeometry

Represents the geometry of a circle or ellipse.

### Properties / fields

| Member | Description |
|---|---|
| `Center` | Gets or sets the center point of the `EllipseGeometry`. <br>Returns: The center point of the `EllipseGeometry`. |
| `RadiusX` | Gets or sets the x-radius value of the `EllipseGeometry`. <br>Returns: The x-radius value of the `EllipseGeometry`. |
| `RadiusY` | Gets or sets the y-radius value of the `EllipseGeometry`. <br>Returns: The y-radius value of the `EllipseGeometry`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterProperty` | Identifies the `Center` dependency property. <br>Returns: The `Center` dependency property identifier. |
| `RadiusXProperty` | Identifies the `RadiusX` dependency property. <br>Returns: The `RadiusX` dependency property identifier. |
| `RadiusYProperty` | Identifies the `RadiusY` dependency property. <br>Returns: The `RadiusY` dependency property identifier. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EllipseGeometry` class. |

## System.Windows.Media.FileSink

Used to save the video capture on Silverlight for Windows Phone to a file.

### Properties / fields

| Member | Description |
|---|---|
| `CaptureSource` | Gets or sets a capture source that is associated with this `FileSink`. <br>Returns: A capture source that is associated with this `FileSink`. |
| `IsolatedStorageFileName` | Gets the name of the associated isolated storage file. <br>Returns: The name of the isolated storage file associated with this `FileSink`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `FileSink` class. |

## System.Windows.Media.FillRule

Specifies how the intersecting areas of `PathFigure` objects contained in a `Geometry` are combined to form the area of the `Geometry`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EvenOdd` | Rule that determines whether a point is in the fill region by drawing a ray from that point to infinity in any direction and counting the number of path segments within the given shape that the ray crosses. If this number is odd, the point is inside; if even, the point is outside. |
| `Nonzero` | Rule that determines whether a point is in the fill region of the path by drawing a ray from that point to infinity in any direction and then examining the places where a segment of the shape crosses the ray. Starting with a count of 0, add one each time a segment crosses the ray from left to right and subtract one each time a path segment crosses the ray from right to left. After counting the crossings, if the result is 0 then the point is outside the path. Otherwise, it is inside. |

## System.Windows.Media.FontFamily

Represents a family of related fonts.

### Properties / fields

| Member | Description |
|---|---|
| `Source` | Gets the font family name that is used to construct the `FontFamily` object. <br>Returns: The font family name of the `FontFamily` object. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `FontFamily` class from the specified font family string. <br>`familyName`: The family name or names that comprise the new `FontFamily`. See "Font Family Grammar" in Remarks. |
| `Equals(Object)` | Gets a value that indicates whether the current font family object and the specified font family object are the same. <br>`o`: The object to compare. <br>Returns: true if `o` is equal to the current `FontFamily` object; otherwise, false. If `o` is not a `FontFamily` object, false is returned. |
| `GetHashCode` | Serves as a hash function for `FontFamily`. <br>Returns: An integer hash value. |
| `ToString` | Returns a string representation of this `FontFamily`. <br>Returns: The input font family string. |

## System.Windows.Media.Fonts

Provides enumeration support for `FontFamily` and `Typeface` objects.

### Properties / fields

| Member | Description |
|---|---|
| `SystemTypefaces` | Gets the collection of `Typeface` objects from the default system font location. <br>Returns: An collection of `Typeface` objects that represent the fonts in the system fonts collection. |

## System.Windows.Media.GeneralTransform

Provides generalized transformation support for objects, such as points and rectangles.

### Properties / fields

| Member | Description |
|---|---|
| `Inverse` | Gets the inverse transformation of this `GeneralTransform`, if possible. <br>Returns: An inverse of this instance, if possible; otherwise null. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GeneralTransform` class. |
| `Transform(Point)` | Transforms the specified point and returns the result. <br>`point`: The point to transform. <br>Returns: The result of transforming . |
| `TransformBounds(Rect)` | When overridden in a derived class, transforms the specified bounding box and returns an axis-aligned bounding box that is exactly large enough to contain it. <br>`rect`: The bounding box to transform. <br>Returns: The smallest axis-aligned bounding box possible that contains the transformed . |
| `TryTransform(Point,Point@)` | When overridden in a derived class, attempts to transform the specified point and returns a value that indicates whether the transformation was successful. <br>`inPoint`: The point to transform. <br>`outPoint`: The result of transforming . <br>Returns: true if was transformed; otherwise, false. |

## System.Windows.Media.Geometry

Provides a base class for objects that define geometric shapes. `Geometry` objects can be used for clipping regions and as geometry definitions for rendering two-dimensional graphic data as a `Path`.

### Properties / fields

| Member | Description |
|---|---|
| `Bounds` | Gets a `Rect` that specifies the axis-aligned bounding box of the `Geometry`. <br>Returns: The axis-aligned bounding box of the `Geometry`. |
| `Empty` | Gets an empty geometry object. <br>Returns: The empty geometry object. |
| `StandardFlatteningTolerance` | Gets the standard tolerance used for polygonal approximation. <br>Returns: The standard tolerance. The default value is 0.25. |
| `Transform` | Gets or sets the `Transform` object applied to a `Geometry`. <br>Returns: The transformation applied to the `Geometry`. Note that this value may be a single `Transform` or a `TransformCollection` cast as a `Transform`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `TransformProperty` | Identifies the `Transform` dependency property. <br>Returns: The identifier for the `Transform` dependency property. |

## System.Windows.Media.GeometryCollection

Represents a collection of `Geometry` objects.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GeometryCollection` class. |

## System.Windows.Media.GeometryGroup

Represents a composite geometry, composed of other `Geometry` objects.

### Properties / fields

| Member | Description |
|---|---|
| `Children` | Gets or sets the `GeometryCollection` that contains the objects that define this `GeometryGroup`. <br>Returns: A collection containing the children of this `GeometryGroup`. |
| `FillRule` | Gets or sets how the intersecting areas of the objects contained in this `GeometryGroup` are combined. <br>Returns: One of the enumeration values that specifies how the intersecting areas are combined to form the resulting area. The default is `EvenOdd`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ChildrenProperty` | Identifies the `Children` dependency property. <br>Returns: The identifier for the `Children` dependency property. |
| `FillRuleProperty` | Identifies the `FillRule` dependency property. <br>Returns: The identifier for the `FillRule` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GeometryGroup` class. |

## System.Windows.Media.GlyphTypeface

Specifies a physical font face that corresponds to a font file on the disk.

### Properties / fields

| Member | Description |
|---|---|
| `FontFileName` | Gets or sets the font file name for the `GlyphTypeface` object. <br>Returns: The font file name for the `GlyphTypeface` object. |
| `Version` | Gets the font face version interpreted from the font's 'NAME' table. <br>Returns: A `Double` value that represents the version. |

## System.Windows.Media.GradientBrush

An abstract class that describes a gradient, composed of gradient stops. Classes that derive from `GradientBrush` describe different ways of interpreting gradient stops.

### Properties / fields

| Member | Description |
|---|---|
| `ColorInterpolationMode` | Gets or sets a `ColorInterpolationMode` enumeration value that specifies how the gradient's colors are interpolated. <br>Returns: Specifies how the colors in a gradient are interpolated. The default is `SRgbLinearInterpolation`. |
| `GradientStops` | Gets or sets the brush's gradient stops. <br>Returns: A collection of the `GradientStop` objects associated with the brush, each of which specifies a color and an offset along the brush's gradient axis. The default is an empty `GradientStopCollection`. |
| `MappingMode` | Gets or sets a `BrushMappingMode` enumeration value that specifies whether the positioning coordinates of the gradient brush are absolute or relative to the output area. <br>Returns: A `BrushMappingMode` value that specifies how to interpret the gradient brush's positioning coordinates. The default is `RelativeToBoundingBox`. |
| `SpreadMethod` | Gets or sets the type of spread method that specifies how to draw a gradient that starts or ends inside the bounds of the object to be painted. <br>Returns: The type of spread method used to paint the gradient. The default is `Pad`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColorInterpolationModeProperty` | Identifies the `ColorInterpolationMode` dependency property. <br>Returns: The identifier for the `ColorInterpolationMode` dependency property. |
| `GradientStopsProperty` | Identifies the `GradientStops` dependency property. <br>Returns: The identifier for the `GradientStops` dependency property. |
| `MappingModeProperty` | Identifies the `MappingMode` dependency property. <br>Returns: The identifier for the `MappingMode` dependency property. |
| `SpreadMethodProperty` | Identifies the `SpreadMethod` dependency property. <br>Returns: The identifier for the `SpreadMethod` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GradientBrush` class. |

## System.Windows.Media.GradientSpreadMethod

Specifies how to draw the gradient outside a gradient brush's gradient vector or space.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Pad` | The color values at the ends of the gradient vector fill the remaining space. |
| `Reflect` | The gradient is repeated in the reverse direction until the space is filled. |
| `Repeat` | The gradient is repeated in the original direction until the space is filled. |

## System.Windows.Media.GradientStop

Describes the location and color of a transition point in a gradient.

### Properties / fields

| Member | Description |
|---|---|
| `Color` | Gets or sets the color of the gradient stop. <br>Returns: The color of the gradient stop. The default is `Transparent`. |
| `Offset` | Gets the location of the gradient stop within the gradient vector. <br>Returns: The relative location of this gradient stop along the gradient vector. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColorProperty` | Identifies the `Color` dependency property. <br>Returns: The identifier for the `Color` dependency property. |
| `OffsetProperty` | Identifies the `Offset` dependency property. <br>Returns: The identifier for the `Offset` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GradientStop` class. |

## System.Windows.Media.GradientStopCollection

Represents a collection of `GradientStop` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GradientStopCollection` class. |

## System.Windows.Media.ImageBrush

Paints an area with an image.

### Properties / fields

| Member | Description |
|---|---|
| `ImageSource` | Gets or sets the image displayed by this `ImageBrush`. <br>Returns: The image displayed by this `ImageBrush`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ImageSourceProperty` | Identifies the `ImageSource` dependency property. <br>Returns: The identifier for the `ImageSource` dependency property. |

### Events

| Member | Description |
|---|---|
| `ImageFailed` | Occurs when there is an error associated with image retrieval or format. |
| `ImageOpened` | Occurs when the image source is downloaded and decoded with no failure. You can use this event to determine the size of an image before rendering it. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ImageBrush` class. |

## System.Windows.Media.ImageSource

Provides an object source type for `Source`, `Source`, and `ImageSource`.

## System.Windows.Media.ImageSourceConverter

Converts a `ImageSource` to and from other data types.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ImageSourceConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the converter can convert an object of the given type to an instance of `ImageSource`. <br>`context`: Type context information used to evaluate conversion. <br>`sourceType`: The type of the source that is being evaluated for conversion. <br>Returns: true if the converter can convert the provided type to an instance of `ImageSource`; otherwise, false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Attempts to convert a specified object to an instance of `ImageSource`. <br>`context`: Type context information used for conversion. <br>`culture`: Cultural information that is respected during conversion. <br>`value`: The object being converted. <br>Returns: A new instance of `ImageSource`. |

## System.Windows.Media.Imaging.BitmapCreateOptions

Specifies initialization options for a bitmap image.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BackgroundCreation` | Causes object to delay initialization until it is necessary., uses image cache for same URIs, downloads images in background and decodes them in background. See Version Notes section. |
| `DelayCreation` | Causes a `BitmapSource` object to delay initialization until it is necessary. This is useful when dealing with collections of images. This is the default value of the `CreateOptions` property in Silverlight. |
| `IgnoreImageCache` | Loads images without using an existing image cache. This option should only be selected when images in a cache need to be refreshed. |
| `None` | No options are specified. This is the NOT the default value for the `CreateOptions` property in Silverlight (`DelayCreation` is the default). |

## System.Windows.Media.Imaging.BitmapImage

Provides the practical object source type for the `Source` and `ImageSource` properties.

### Properties / fields

| Member | Description |
|---|---|
| `CreateOptions` | Gets or sets the `BitmapCreateOptions` for a `BitmapImage`. <br>Returns: The `BitmapCreateOptions` used for this `BitmapImage`. The default is `None`. |
| `UriSource` | Gets or sets the URI of the graphics source file that generated this `BitmapImage`. <br>Returns: The URI of the graphics source file that generated this `BitmapImage`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CreateOptionsProperty` | Identifies the `CreateOptions` dependency property. <br>Returns: The identifier for the `CreateOptions` dependency property. |
| `UriSourceProperty` | Identifies the `UriSource` dependency property. |

### Events

| Member | Description |
|---|---|
| `DownloadProgress` | Occurs when a significant change has occurred in the download progress of the `BitmapImage` content. |
| `ImageFailed` | Occurs when there is an error associated with image retrieval or format. |
| `ImageOpened` | Occurs when the image source is downloaded and decoded with no failure. You can use this event to determine the size of an image before rendering it. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BitmapImage` class. |
| `Constructor(Uri)` | Initializes a new instance of the `BitmapImage` class, using the supplied URI. <br>`uriSource`: The URI that references the source graphics file for the image. |

## System.Windows.Media.Imaging.BitmapSource

Provides a source object for properties that use a bitmap.

### Properties / fields

| Member | Description |
|---|---|
| `PixelHeight` | Gets the height of the bitmap in pixels. <br>Returns: The height of the bitmap in pixels. |
| `PixelWidth` | Gets the width of the bitmap in pixels. <br>Returns: The width of the bitmap in pixels. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PixelHeightProperty` | Identifies the `PixelHeight` dependency property. <br>Returns: The identifier for the `PixelHeight` dependency property. |
| `PixelWidthProperty` | Identifies the `PixelWidth` dependency property. <br>Returns: The identifier for the `PixelWidth` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BitmapSource` class. |
| `SetSource(IO.Stream)` | Sets the source of the `BitmapSource`. <br>`streamSource`: The stream to set the source to. |

## System.Windows.Media.Imaging.BitmapSourceExtensions

Provides extension methods for the `BitmapSource` class to support 3D graphic texture creation.

### Methods

| Member | Description |
|---|---|
| `CopyTo(Media.Imaging.BitmapSource,Microsoft.Xna.Framework.Graphics.Texture2D)` | Copies a bitmap image to a texture. <br>`bitmap`: The bitmap image object. <br>`dest`: The texture. |
| `CopyTo(Media.Imaging.BitmapSource,Microsoft.Xna.Framework.Graphics.Texture2D,Int32,Nullable{Microsoft.Xna.Framework.Rectangle},Int32,Int32)` | Copies a sub-region of bitmap to a sub-region of the texture. <br>`bitmap`: The bitmap image object. <br>`dest`: The texture. <br>`level`: Mipmap level. <br>`srcRect`: The region of the bitmap image to use. <br>`destX`: The x-coordinate of the position of the texture to copy the sub-region of the bitmap to. <br>`destY`: The y-coordinate of the position of the texture to copy the sub-region of the bitmap to. |
| `CopyTo(Media.Imaging.BitmapSource,Microsoft.Xna.Framework.Graphics.TextureCube,Microsoft.Xna.Framework.Graphics.CubeMapFace)` | Copies a bitmap image to a cube map face. <br>`bitmap`: The bitmap image object. <br>`dest`: The texture. <br>`face`: The cube face. |
| `CopyTo(Media.Imaging.BitmapSource,Microsoft.Xna.Framework.Graphics.TextureCube,Microsoft.Xna.Framework.Graphics.CubeMapFace,Int32,Nullable{Microsoft.Xna.Framework.Rectangle},Int32,Int32)` | Copies a sub-region of bitmap to a sub-region of the cube map face. <br>`bitmap`: The bitmap image object. <br>`dest`: The texture. <br>`face`: The face of the cube. <br>`level`: Mipmap level. <br>`srcRect`: The region of the bitmap image to use. <br>`destX`: The x-coordinate of the position of the to copy the sub-region of the bitmap to. <br>`destY`: The y-coordinate of the position of the texture to copy the sub-region of the bitmap to. |

## System.Windows.Media.Imaging.DownloadProgressEventArgs

Provides event data for the `DownloadProgress` event.

### Properties / fields

| Member | Description |
|---|---|
| `Progress` | Gets download progress as a value that is between 0 and 100. <br>Returns: The download progress. A value of 0 indicates no progress; 100 indicates the download is complete. |

## System.Windows.Media.Imaging.WriteableBitmap

Provides a `BitmapSource` that can be written to and updated.

### Properties / fields

| Member | Description |
|---|---|
| `Pixels` | Gets an array representing the 2-D texture of the bitmap. <br>Returns: An array of integers representing the 2-D texture of the bitmap. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Int32,Int32)` | Initializes a new instance of the `WriteableBitmap` class using the provided dimensions. <br>`pixelWidth`: The width of the bitmap. <br>`pixelHeight`: The height of the bitmap. |
| `Constructor(Media.Imaging.BitmapSource)` | Initializes a new instance of the `WriteableBitmap` class using the provided `BitmapSource`. <br>`source`: The `BitmapSource` to use for initialization. |
| `Constructor(UIElement,Media.Transform)` | Initializes a new instance of the `WriteableBitmap` class using the provided element and transform. <br>`element`: The desired element to be rendered within the bitmap. <br>`transform`: The transform the user wants to apply to the element as the last step before drawing into the bitmap. This is particularly interesting for you if you want the bitmap to respect its transform. This value can be null. |
| `Invalidate` | Requests a draw or redraw of the entire bitmap. |
| `Render(UIElement,Media.Transform)` | Renders an element within the bitmap. <br>`element`: The element to be rendered within the bitmap. <br>`transform`: The transform to apply to the element before drawing into the bitmap. If an empty transform is supplied, the bits representing the element show up at the same offset as if they were placed within their parent. |

## System.Windows.Media.ImplicitInputBrush

Represents the input bitmap to a shader.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ImplicitInputBrush` class. |

## System.Windows.Media.JoinDomainCompletedEventArgs

Provides data for the `JoinDomainCompleted` event.

## System.Windows.Media.LeaveDomainCompletedEventArgs

Provides data for the `LeaveDomainCompleted` event.

## System.Windows.Media.LicenseAcquirer

This class handles acquiring licenses for DRM encrypted content.

### Properties / fields

| Member | Description |
|---|---|
| `ChallengeCustomData` | Gets or sets a string that contains service-specific data to be conveyed to the license server without implementing manual license acquisition. <br>Returns: A string that contains service-specific data to be conveyed to the license server without implementing manual license acquisition. The default value is null. |
| `DomainAcquirer` | Gets or sets a `DomainAcquirer` to handle Join Domain requests that are triggered from License Server exceptions (DomainRequired or RenewDomain). <br>Returns: A `DomainAcquirer` to handle Join Domain requests that are triggered from License Server exceptions (DomainRequired or RenewDomain). |
| `LicenseServerUriOverride` | Gets or sets a Uniform Resource Identifier (URI) value that overrides whatever the license server URI is in the content header. <br>Returns: The URI that overrides the URI that is provided in the content header (if any). If this value is null, the system uses whatever license server URI is in the content header. The default value is null. |

### Events

| Member | Description |
|---|---|
| `AcquireLicenseCompleted` | Occurs when the license acquisition completes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LicenseAcquirer` class. |
| `AcquireLicenseAsync(Byte[])` | Starts the license acquisition process. <br>`mediaHeader`: The header that is used for the license acquisition. |
| `AcquireLicenseAsync(Guid,Media.ContentKeyType,Guid)` | Starts the license acquisition process. <br>`keyId`: The key identifier of the file you are attempting to get the license for. <br>`keyType`: The type of encryption standard that is used by the key identifier. <br>`serviceId`: The service ID. This parameter is required only if you want to obtain a domain-bound license. |
| `AcquireLicenseAsync(IO.Stream)` | Starts the license acquisition process by specifying a MediaStream. <br>`mediaStream`: The stream that is used for the license acquisition. |
| `CancelAsync` | Cancels a license acquisition. |
| `OnAcquireLicense(IO.Stream,Uri)` | The default implementation of this method calls into the `MediaElement` to acquire a license. Developers should override this method if they want to handle the license acquisition themselves. <br>`licenseChallenge`: The SOAP license challenge. <br>`licenseServerUri`: The URI of the License Server. In the case of PlayReady content with a license server, this value is the value in the PlayReady header. In the case of WMDRM content and PlayReady content without a license server specified in the header, this value is null unless the `LicenseServerUriOverride` property has been set. If the `LicenseServerUriOverride` property is set, its value is passed in as the `licenseServerUri`. |
| `OnCancel` | This method is overridden in a derived class to provide a standard way of doing cancellation during manual license acquisition. |
| `SetLicenseResponse(IO.Stream)` | Sets the license response by passing the SOAP body from the HTTP response to the license challenge. <br>`licenseResponse`: The SOAP body from the HTTP response to the license challenge. The SOAP body is passed in as a stream and contains the entire SOAP response. |

## System.Windows.Media.LicenseManagement

A static class that is used to return licenses from the persistent license store.

### Properties / fields

| Member | Description |
|---|---|
| `VideoOutputConnectors` | Gets all the data about connector types from the graphics card, and also gets the output protection schemes for which Silverlight can engage each video output. <br>Returns: The data on connector types and the output protection schemes that Silverlight can engage for each video output. Each capture device that is returned in this list may have multiple configurations. |

### Methods

| Member | Description |
|---|---|
| `SelectLicenses(Guid)` | Returns licenses from the persistent license store on the client based on the provided key identifier. <br>`keyId`: The key ID for the license to return. <br>Returns: The licenses from the persistent license store on the client based on the provided key identifier. If no IBX is present on the system, it is assumed the user has no licenses, and an empty enumerator is returned from this method call. |
| `SelectLicenses(IO.Stream)` | Returns licenses from the persistent license store on the client based on a media stream, which contains a content header. <br>`mediaStream`: The stream for the license to return. <br>Returns: The licenses from the persistent license store on the client based on a media stream, which contains a content header. If no IBX is present on the system, it is assumed the user has no licenses and an empty enumerator is returned from this method call. |

## System.Windows.Media.LineGeometry

Represents the geometry of a line.

### Properties / fields

| Member | Description |
|---|---|
| `EndPoint` | Gets or sets the end point of a line. <br>Returns: The end point of the line. The default is a `Point` with value 0,0. |
| `StartPoint` | Gets or sets the start point of the line. <br>Returns: The start point of the line. The default is a `Point` with value 0,0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EndPointProperty` | Identifies the `EndPoint` dependency property. <br>Returns: The identifier for the `EndPoint` dependency property. |
| `StartPointProperty` | Identifies the `StartPoint` dependency property. <br>Returns: The identifier for the `StartPoint` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LineGeometry` class that has no length. |

## System.Windows.Media.LineSegment

Represents a line drawn between two points, which can be part of a `PathFigure` within `Path` data.

### Properties / fields

| Member | Description |
|---|---|
| `Point` | Gets or sets the end point of the line segment. <br>Returns: The end point of the line segment. The default is a `Point` with value 0,0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PointProperty` | Identifies the `Point` dependency property. <br>Returns: The identifier for the `Point` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LineSegment` class. |

## System.Windows.Media.LinearGradientBrush

Paints an area with a linear gradient.

### Properties / fields

| Member | Description |
|---|---|
| `EndPoint` | Gets or sets the ending two-dimensional coordinates of the linear gradient. <br>Returns: The ending two-dimensional coordinates of the linear gradient. The default is a `Point` with value 1,1. |
| `StartPoint` | Gets or sets the starting two-dimensional coordinates of the linear gradient. <br>Returns: The starting two-dimensional coordinates for the linear gradient. The default is a `Point` with value 0,0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EndPointProperty` | Identifies the `EndPoint` dependency property. <br>Returns: The identifier for the `EndPoint` dependency property. |
| `StartPointProperty` | Identifies the `StartPoint` dependency property. <br>Returns: The identifier for the `StartPoint` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LinearGradientBrush` class. |
| `Constructor(Media.GradientStopCollection,Double)` | Initializes a new instance of the `LinearGradientBrush` class that has the specified `GradientStopCollection` and angle. <br>`gradientStopCollection`: The `GradientStops` to set on this brush. <br>`angle`: A `Double` that represents the angle, in degrees, of the gradient. A value of 0 creates a horizontal gradient, and a value of 90 creates a vertical gradient. Negative values are permitted, as are values over 360 (are treated as mod 360). |

## System.Windows.Media.LogReadyRoutedEventArgs

Provides data for the `LogReady` event.

### Properties / fields

| Member | Description |
|---|---|
| `Log` | Gets an XML string in the Windows Media Log format that contains the fields listed in the LogFields section. <br>Returns: The XML string in the Windows Media Log format that contains the fields listed in the LogFields section. |
| `LogSource` | Gets a value that indicates why the log was generated. <br>Returns: One of the enumeration values that indicates why the log was generated. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LogReadyRoutedEventArgs` class. |

## System.Windows.Media.LogReadyRoutedEventHandler

Represents the method that will handle the `LogReady` event.

## System.Windows.Media.LogSource

Indicates the reason that a media log was generated.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `EndOfStream` | The media reached the end of its stream. |
| `MediaElementShutdown` | The runtime shut down. |
| `Pause` | The media paused. |
| `RequestLog` | The `RequestLog` method was called. |
| `RuntimeShutdown` | The runtime shut down. |
| `Seek` | A seek operation occurred on the media; that is, playback was moved forward or backward. |
| `SourceChanged` | The source of the media changed. |
| `Stop` | The media stopped. |

## System.Windows.Media.Matrix

Represents a 3x3 affine transformation matrix used for transformations in two-dimensional space.

### Properties / fields

| Member | Description |
|---|---|
| `Identity` | Gets an identity `Matrix`. <br>Returns: An identity matrix. |
| `IsIdentity` | Gets a value that indicates whether this `Matrix` structure is an identity matrix. <br>Returns: true if the `Matrix` structure is an identity matrix; otherwise, false. The default is true. |
| `M11` | Gets or sets the value of the first row and first column of this `Matrix` structure. <br>Returns: The value of the first row and first column of this `Matrix`. The default value is 1. |
| `M12` | Gets or sets the value of the first row and second column of this `Matrix` structure. <br>Returns: The value of the first row and second column of this `Matrix`. The default value is 0. |
| `M21` | Gets or sets the value of the second row and first column of this `Matrix` structure. <br>Returns: The value of the second row and first column of this `Matrix`. The default value is 0. |
| `M22` | Gets or sets the value of the second row and second column of this `Matrix` structure. <br>Returns: The value of the second row and second column of this `Matrix` structure. The default value is 1. |
| `OffsetX` | Gets or sets the value of the third row and first column of this `Matrix` structure. <br>Returns: The value of the third row and first column of this `Matrix` structure. The default value is 0. |
| `OffsetY` | Gets or sets the value of the third row and second column of this `Matrix` structure. <br>Returns: The value of the third row and second column of this `Matrix` structure. The default value is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double,Double,Double,Double,Double)` | Initializes a `Matrix` structure. <br>`m11`: The `Matrix` structure's `M11` coefficient. <br>`m12`: The `Matrix` structure's `M12` coefficient. <br>`m21`: The `Matrix` structure's `M21` coefficient. <br>`m22`: The `Matrix` structure's `M22` coefficient. <br>`offsetX`: The `Matrix` structure's `OffsetX` coefficient. <br>`offsetY`: The `Matrix` structure's `OffsetY` coefficient. |
| `Equals(Object)` | Determines whether the specified `Object` is a `Matrix` structure that is identical to this `Matrix`. <br>`o`: The `Object` to compare. <br>Returns: true if `o` is a `Matrix` structure that is identical to this `Matrix` structure; otherwise, false. |
| `Equals(Media.Matrix)` | Determines whether the specified `Matrix` structure is identical to this instance. <br>`value`: The instance of `Matrix` to compare to this instance. <br>Returns: true if instances are equal; otherwise, false. |
| `GetHashCode` | Returns the hash code for this `Matrix` structure. <br>Returns: The hash code for this instance. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `IFormatProvider)`. <br>`format`: The string specifying the format to use. -or- null to use the default format defined for the type of the IFormattable implementation. <br>`provider`: The IFormatProvider to use to format the value. -or- null to obtain the numeric format information from the current locale setting of the operating system. <br>Returns: A string containing the value of the current instance in the specified format. |
| `ToString` | Creates a `String` representation of this `Matrix` structure. <br>Returns: A `String` containing the `M11`, `M12`, `M21`, `M22`, `OffsetX`, and `OffsetY` values of this `Matrix`. |
| `ToString(IFormatProvider)` | Creates a `String` representation of this `Matrix` structure with culture-specific formatting information. <br>`provider`: The culture-specific formatting information. <br>Returns: A `String` containing the `M11`, `M12`, `M21`, `M22`, `OffsetX`, and `OffsetY` values of this `Matrix`. |
| `Transform(Point)` | Transforms the specified point by the `Matrix` and returns the result. <br>`point`: The point to transform. <br>Returns: The result of transforming `point` by this `Matrix`. |
| `op_Equality(Media.Matrix,Media.Matrix)` | Determines whether the two specified `Matrix` structures are identical. <br>`matrix1`: The first `Matrix` structure to compare. <br>`matrix2`: The second `Matrix` structure to compare. <br>Returns: true if and are identical; otherwise, false. |
| `op_Inequality(Media.Matrix,Media.Matrix)` | Determines whether the two specified `Matrix` structures are not identical. <br>`matrix1`: The first `Matrix` structure to compare. <br>`matrix2`: The second `Matrix` structure to compare. <br>Returns: true if and are not identical; otherwise, false. |

## System.Windows.Media.Matrix3DProjection

Enables you to apply a `Matrix3D` to an object.

### Properties / fields

| Member | Description |
|---|---|
| `ProjectionMatrix` | Gets or sets the `Matrix3D` that is used for the projection that is applied to the object. <br>Returns: The `Matrix3D` that is used for the projection that is applied to the object. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ProjectionMatrixProperty` | Identifies the `ProjectionMatrix` dependency property. <br>Returns: The identifier for the `ProjectionMatrix` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of a `Matrix3DProjection` class. |

## System.Windows.Media.MatrixTransform

Creates an arbitrary affine matrix transformation that is used to manipulate objects or coordinate systems in a two-dimensional plane.

### Properties / fields

| Member | Description |
|---|---|
| `Matrix` | Gets or sets the `Matrix` that defines this transformation. <br>Returns: The `Matrix` structure that defines this transformation. The default is an identity `Matrix`. An identity matrix has a value of 1 in coefficients [1,1], [2,2], and [3,3]; and a value of 0 in the rest of the coefficients. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `MatrixProperty` | Identifies the `Matrix` dependency property. <br>Returns: The identifier for the `Matrix` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `MatrixTransform` class. |

## System.Windows.Media.Media3D.Matrix3D

Represents a 4 × 4 matrix that is used for transformations in a three-dimensional (3-D) space.

### Properties / fields

| Member | Description |
|---|---|
| `HasInverse` | Gets a value that indicates whether this `Matrix3D` is invertible. <br>Returns: true if the `Matrix3D` has an inverse; otherwise, false. The default value is true. |
| `Identity` | Changes a `Matrix3D` structure into an identity `Matrix3D`. <br>Returns: The identity `Matrix3D`. |
| `IsIdentity` | Determines whether this `Matrix3D` structure is an identity `Matrix3D`. <br>Returns: true if the `Matrix3D` is an identity `Matrix3D`; otherwise, false. The default value is true. |
| `M11` | Gets or sets the value of the first row and first column of this `Matrix3D`. <br>Returns: The value of the first row and first column of this `Matrix3D` structure. |
| `M12` | Gets or sets the value of the first row and second column of this `Matrix3D`. <br>Returns: The value of the first row and second column of this `Matrix3D`. |
| `M13` | Gets or sets the value of the first row and third column of this `Matrix3D`. <br>Returns: The value of the first row and third column of this `Matrix3D`. |
| `M14` | Gets or sets the value of the first row and fourth column of this `Matrix3D`. <br>Returns: The value of the first row and fourth column of this `Matrix3D`. |
| `M21` | Gets or sets the value of the second row and first column of this `Matrix3D`. <br>Returns: The value of the second row and first column of this `Matrix3D`. |
| `M22` | Gets or sets the value of the second row and second column of this `Matrix3D`. <br>Returns: The value of the second row and second column of this `Matrix3D`. |
| `M23` | Gets or sets the value of the second row and third column of this `Matrix3D`. <br>Returns: The value of the second row and third column of this `Matrix3D`. |
| `M24` | Gets or sets the value of the second row and fourth column of this `Matrix3D`. <br>Returns: The value of the second row and fourth column of this `Matrix3D`. |
| `M31` | Gets or sets the value of the third row and first column of this `Matrix3D`. <br>Returns: The value of the third row and first column of this `Matrix3D`. |
| `M32` | Gets or sets the value of the third row and second column of this `Matrix3D`. <br>Returns: The value of the third row and second column of this `Matrix3D`. |
| `M33` | Gets or sets the value of the third row and third column of this `Matrix3D`. <br>Returns: The value of the third row and third column of this `Matrix3D`. |
| `M34` | Gets or sets the value of the third row and fourth column of this `Matrix3D`. <br>Returns: The value of the third row and fourth column of this `Matrix3D`. |
| `M44` | Gets or sets the value of the fourth row and fourth column of this `Matrix3D`. <br>Returns: The value of the fourth row and fourth column of this `Matrix3D`. |
| `OffsetX` | Gets or sets the value of the fourth row and first column of this `Matrix3D`. <br>Returns: The value of the fourth row and first column of this `Matrix3D`. |
| `OffsetY` | Gets or sets the value of the fourth row and second column of this `Matrix3D`. <br>Returns: The value of the fourth row and second column of this `Matrix3D`. |
| `OffsetZ` | Gets or sets the value of the fourth row and third column of this `Matrix3D`. <br>Returns: The value of the fourth row and third column of this `Matrix3D`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double,Double)` | Initializes a new instance of the `Matrix3D` class. <br>`m11`: The value of the (1,1) field of the new matrix. <br>`m12`: The value of the (1,2) field of the new matrix. <br>`m13`: The value of the (1,3) field of the new matrix. <br>`m14`: The value of the (1,4) field of the new matrix. <br>`m21`: The value of the (2,1) field of the new matrix. <br>`m22`: The value of the (2,2) field of the new matrix. <br>`m23`: The value of the (2,3) field of the new matrix. <br>`m24`: The value of the (2,4) field of the new matrix. <br>`m31`: The value of the (3,1) field of the new matrix. <br>`m32`: The value of the (3,2) field of the new matrix. <br>`m33`: The value of the (3,3) field of the new matrix. <br>`m34`: The value of the (3,4) field of the new matrix. <br>`offsetX`: The value of the X offset field of the new matrix. <br>`offsetY`: The value of the Y offset field of the new matrix. <br>`offsetZ`: The value of the Z offset field of the new matrix. <br>`m44`: The value of the (4,4) field of the new matrix. |
| `Equals(Object)` | Tests equality between two matrices. <br>`o`: The object to test for equality. <br>Returns: true if the matrices are equal; otherwise, false. |
| `Equals(Media.Media3D.Matrix3D)` | Tests equality between two matrices. <br>`value`: The `Matrix3D` to compare to. <br>Returns: true if the matrices are equal; otherwise, false. |
| `GetHashCode` | Returns the hash code for this matrix. <br>Returns: An integer that specifies the hash code for this matrix. |
| `Invert` | Inverts this `Matrix3D` structure. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `ToString`. <br>`format`: The format to use. <br>`provider`: The provider to use. <br>Returns: The value of the current instance in the specified format. |
| `ToString` | Creates a string representation of this `Matrix3D`. <br>Returns: A string representation of this `Matrix3D`. |
| `ToString(IFormatProvider)` | Creates a string representation of this `Matrix3D`. <br>`provider`: Culture-specified formatting information. <br>Returns: The string representation of this `Matrix3D`. |
| `op_Equality(Media.Media3D.Matrix3D,Media.Media3D.Matrix3D)` | Compares two `Matrix3D` instances for exact equality. <br>`matrix1`: The first matrix to compare. <br>`matrix2`: The second matrix to compare. <br>Returns: true if the matrices are equal; otherwise, false. |
| `op_Inequality(Media.Media3D.Matrix3D,Media.Media3D.Matrix3D)` | Compares two `Matrix3D` instances for inequality. <br>`matrix1`: The first matrix to compare. <br>`matrix2`: The second matrix to compare. <br>Returns: true if the matrices are different; otherwise, false. |
| `op_Multiply(Media.Media3D.Matrix3D,Media.Media3D.Matrix3D)` | Multiplies the specified matrices. <br>`matrix1`: The matrix to multiply. <br>`matrix2`: The matrix by which the first matrix is multiplied. <br>Returns: The `Matrix3D` that is the result of multiplication. |

## System.Windows.Media.MediaElementState

Defines the potential states of a `MediaElement` object.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AcquiringLicense` | The `MediaElement` is acquiring a license required to play DRM protected content. Once `Uri)` has been called, the `MediaElement` will remain in this state until `Stream)` has been called. |
| `Buffering` | The `MediaElement` is loading the media for playback. Its `Position` does not advance during this state. If the `MediaElement` was already playing video, it continues to display the last displayed frame. |
| `Closed` | The `MediaElement` contains no media. The `MediaElement` displays a transparent frame. |
| `Individualizing` | The `MediaElement` is in the process of ensuring that proper individualization components (only applicable when playing DRM protected content) are installed on the user's computer. See Digital Rights Management (DRM) for more information. |
| `Opening` | The `MediaElement` is validating and attempting to open the Uniform Resource Identifier (URI) specified by its `Source` property. While in this state, the `MediaElement` queues any `Play`, `Pause`, or `Stop` commands it receives and processes them if the media is successfully opened. |
| `Paused` | The `MediaElement` does not advance its `Position`. If the `MediaElement` was playing video, it continues to display the current frame. |
| `Playing` | The `MediaElement` is playing the media specified by its source property. Its `Position` advances forward. |
| `Stopped` | The `MediaElement` contains media but is not playing or paused. Its `Position` is 0 and does not advance. If the loaded media is video, the `MediaElement` displays the first frame. |

## System.Windows.Media.MediaLicense

An instance of the license class representing a logical license.

### Properties / fields

| Member | Description |
|---|---|
| `ExpirationDate` | Gets the expiration date of the `MediaLicense` class instance. <br>Returns: The expiration date of the `MediaLicense` class instance. |
| `ZeyId` | Gets the content key identifier of the `MediaLicense`. <br>Returns: The content key identifier of the `MediaLicense`. If the `MediaLicense` instance represents a license, then this property returns the content key identifier of the license itself. If the class instance represents a license chain, then it is the content key identifier of the leaf license. For more information about license chains and leaf licenses, see Digital Rights Management (DRM). Since content also contains the key identifier for the key needed to decrypt the content, this value is used to pair a license with the content for decryption. |
| `UplinkKeyId` | Gets the uplink key identifier of the highest license in the chain with an uplink key identifier. <br>Returns: The uplink key identifier of the highest license in the chain with an uplink key identifier. If the `MediaLicense` class contains a root license or a simple license, then this property returns Guid.Empty. |
| `Usable` | Gets a value that indicates whether the license or license chain is usable for playback. <br>Returns: The value that indicates whether the license or license chain is usable for playback. |

## System.Windows.Media.MediaSampleAttributeKeys

This enumeration is used in a dictionary of attributes for media samples.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DRMInitializationVector` | Provides data about the media sample that is needed to decrypt it. |
| `DRMSubSampleMapping` | Provides data about which portions of a media sample are encrypted. This value is a B64 encoded array of one or more 32bit pairs. The first value in a pair is the length of encrypted data, the second is the length of clear (non-encrypted) data. If supplied, the array values must total to the length of the ntire sample. If not supplied, any encryption is assume to cover the entire sample. the length of encrypted data and clear data (not encrypted data) in an H264 media sample.This attribute is supported for H.264 on both Windows Phone 7 and Silverlight 4. |
| `FrameHeight` | The height of the video frame. If this attribute is not specified, the width of the sample is assumed to be the height value defined on the `MediaStreamAttributeKeys` passed into to the `MediaStreamDescription})` method. |
| `FrameWidth` | The width of the video frame. If this attribute is not specified, the width of the sample is assumed to be the width value defined on the `MediaStreamAttributeKeys` passed into to the `MediaStreamDescription})` method. |
| `ZeyFrameFlag` | For video samples, the presence of this attribute indicates the sample is a is a keyframe. Silverlight for Windows Phone For audio samples, the presence of this attribute indicates a discontinuity (Silverlight for Windows Phone only). |

## System.Windows.Media.MediaSourceAttributesKeys

Describes the media source.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CanSeek` | A Boolean value that describes whether this source can seek. |
| `DRMHeader` | DRM data that the pipeline needs to initialize and decrypt correctly. This is the DRM header represented as a string. |
| `Duration` | The length of playback time of this source as an integer in 100-nanosecond increments. |

## System.Windows.Media.MediaStreamAttributeKeys

This enumeration is used in a dictionary of attributes for media streams.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CodecPrivateData` | Codec data that the pipeline needs to initialize and render correctly. For video, this is other header information. For audio, this is the base16-encoded WaveFormatEx structure. For more information on CodecPrivateDate, see Implementing MediaStream Sources and Media in Silverlight for Windows Phone. |
| `Height` | The maximum height of reported video frames for this stream and the default width to render them at. |
| `VideoFourCC` | Data needed to instantiate a video codec. This is the four-character value also known as a FourCC. For more information on VideoFourCC CodecPrivateDate, see Implementing MediaStream Sources and Media in Silverlight for Windows Phone. |
| `Width` | The maximum width of reported video frames for this stream and the default width to render them at. |

## System.Windows.Media.MediaStreamDescription

This class describes a media stream in enough detail to initialize the `MediaElement` and the underlying media pipeline.

### Properties / fields

| Member | Description |
|---|---|
| `MediaAttributes` | Gets a collection of attributes about the media stream. This collection only contains attributes that are used to initialize the media pipeline and the `MediaElement`. <br>Returns: A collection of attributes about the media stream. The default value is Empty IDictionary<MediaStreamAttributeKeys, string>. |
| `StreamId` | Gets the stream ID of the stream being described. <br>Returns: The stream ID of the stream being described. |
| `Type` | Gets the type of stream that is being described which is either `Audio` or `Video`. <br>Returns: The type of stream that is being described which is either `Audio` or `Video`. The default value is `Video`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Media.MediaStreamType,IDictionary{Media.MediaStreamAttributeKeys,String})` | Initializes a new instance of the `MediaStreamDescription` class with the specified values. <br>`type`: A value from the enumeration, which is either audio or video. <br>`mediaStreamAttributes`: A collection of pairs describing other attributes of the overall Media Stream. |

## System.Windows.Media.MediaStreamSample

This class describes a media sample in enough detail to allow the `MediaElement` and underlying pipeline to render the sample.

### Properties / fields

| Member | Description |
|---|---|
| `Attributes` | Gets a collection of attributes describing to the `MediaElement` and media pipeline how to render this sample. <br>Returns: A collection of attributes describing to the `MediaElement` and media pipeline how to render this sample. The default value is Empty IDictionary<MediaSampleAttributeKeys, string> |
| `Count` | Gets the number of bytes into the stream where the sample ends. <br>Returns: The number of bytes into the stream where the sample ends. The default value is 0. |
| `Duration` | Gets the duration of the sample. <br>Returns: The duration of the sample in 10 nanosecond units. |
| `MediaStreamDescription` | Gets the description of the stream that this sample came from. <br>Returns: The description of the stream that this sample came from. The default value is null. |
| `Offset` | Gets the byte offset into the stream where the sample comes from. For sample streams that contain a header as well, this is set to where the actual sample begins. <br>Returns: The byte offset into the stream where the sample comes from. The default value is 0. |
| `Stream` | Gets the stream of data representing the sample. <br>Returns: The stream of data representing the sample. The default value is null. |
| `Timestamp` | Gets the time at which a sample should be rendered as measured in 100 nanosecond increments. <br>Returns: The time at which a sample should be rendered as measured in 100 nanosecond increments. The default value is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Media.MediaStreamDescription,IO.Stream,Int64,Int64,Int64,IDictionary{Media.MediaSampleAttributeKeys,String})` | Instantiates a new instance of the `MediaStreamSample` class. <br>`mediaStreamDescription`: A description of the stream this sample was pulled from. <br>`stream`: A stream containing the desired media sample. Set to null to report the end of a stream. <br>`offset`: The offset into the stream where the actual sample data begins. <br>`count`: The number of bytes that comprises the sample data. <br>`timestamp`: The time from the beginning of the media file at which this sample should be rendered as expressed using 100 nanosecond increments. <br>`attributes`: A collection of pairs describing other attributes of the media sample. |
| `Constructor(Media.MediaStreamDescription,IO.Stream,Int64,Int64,Int64,Int64,IDictionary{Media.MediaSampleAttributeKeys,String})` | Instantiates a new instance of the `MediaStreamSample` class. <br>`mediaStreamDescription`: A description of the stream this sample was pulled from. <br>`stream`: A stream containing the desired media sample. Set to null to report the end of a stream. <br>`offset`: The offset into the stream where the actual sample data begins. <br>`count`: The number of bytes that comprises the sample data. <br>`timestamp`: The time from the beginning of the media file at which this sample should be rendered as expressed using 100 nanosecond increments. <br>`duration`: The duration of the sample. <br>`attributes`: A collection of pairs describing other attributes of the media sample. |

## System.Windows.Media.MediaStreamSource

A MediaStreamSource delivers media samples directly to the media pipeline and is most often used to enable the `MediaElement` to use a container format not natively supported by Silverlight.

### Properties / fields

| Member | Description |
|---|---|
| `AudioBufferLength` | Gets or sets the length of the audio buffer. <br>Returns: The length of the audio buffer in milliseconds. The value must be between 15 and 1000. The default value is 1000, which is a one second buffer. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `MediaStreamSource` class. |
| `CloseMedia` | The `MediaElement` can call this method when going through normal shutdown or as a result of an error. This lets the developer perform any needed cleanup of the `MediaStreamSource`. |
| `ErrorOccurred(String)` | Developers call this method whenever an unrecoverable error has occurred in the `MediaStreamSource`. This will cause the `MediaFailed` event to be raised. <br>`errorDescription`: A string describing the error. |
| `GetDiagnosticAsync(Media.MediaStreamSourceDiagnosticKind)` | The `MediaElement` can call this method to request information about the `MediaStreamSource`. Developers respond to this method by calling `Int64)`. <br>`diagnosticKind`: A member of the `MediaStreamSourceDiagnosticKind` enumeration describing what type of information is desired. |
| `GetSampleAsync(Media.MediaStreamType)` | The `MediaElement` calls this method to ask the `MediaStreamSource` to prepare the next `MediaStreamSample` of the requested stream type for the media pipeline. Developers can respond to this method by calling either `MediaStreamSample)` or `Double)`. <br>`mediaStreamType`: The description of the stream that the next sample should come from which will be either `Audio` or `Video`. |
| `OpenMediaAsync` | The `MediaElement` calls this method to ask the `MediaStreamSource` to open the media. |
| `ReportGetDiagnosticCompleted(Media.MediaStreamSourceDiagnosticKind,Int64)` | Developers call this method, in response to `MediaStreamSourceDiagnosticKind)`, to pass the requested diagnostic information to the `MediaElement`. <br>`diagnosticKind`: The type of diagnostic that you are returning a value for. <br>`diagnosticValue`: The value associated with the diagnostics that was returned. |
| `ReportGetSampleCompleted(Media.MediaStreamSample)` | Developers call this method in response to `MediaStreamType)` to give the `MediaElement` the next media sample to be rendered, or to report the end of a stream. <br>`mediaStreamSample`: The description of the media stream that this sample came from. Passing null`MediaStreamSample` object indicates that the stream has ended. |
| `ReportGetSampleProgress(Double)` | Developers call this method in response to `MediaStreamType)` to inform the `MediaElement` that it will not return a sample right now, because the `MediaStreamSource` needs to refill its buffers, and to allow the `MediaElement` to transition to the `Buffering` state. <br>`bufferingProgress`: A value between 0 and 1 indicating what percentage of the buffer is filled. |
| `ReportOpenMediaCompleted(IDictionary{Media.MediaSourceAttributesKeys,String},IEnumerable{Media.MediaStreamDescription})` | Developers call this method in response to `OpenMediaAsync` to inform the `MediaElement` that the `MediaStreamSource` has been opened and to supply information about the streams it contains. <br>`mediaStreamAttributes`: A collection of attributes describing features of the entire media source. Currently supported attributes are listed in the `MediaSourceAttributesKeys`. <br>`availableMediaStreams`: A description of each audio and video stream contained within the content. |
| `ReportSeekCompleted(Int64)` | Developers call this method in response to `Int64)` to inform the `MediaElement` that the `MediaStreamSource` has finished the requested position change and that future calls to `MediaStreamSample)` will return samples from that point in the media. <br>`timeSeekedTo`: The time as represented by 100 nanosecond increments where the actual seek took place. This is typically measured from the beginning of the media file. |
| `ReportSetPlaybackRateCompleted(Single)` | Notifies the `MediaElement` that the playback rate been set to the specified value. <br>`rate`: The new play back rate |
| `ReportSwitchMediaStreamCompleted(Media.MediaStreamDescription)` | Developers call this method in response to `MediaStreamDescription)` to inform the `MediaElement` that the `MediaStreamSource` has completed the requested stream switch and that samples returned will now be from the requested stream instead of the original stream. Note, that this is meant for the multiple audio stream case, for example language tracks, and not the adaptive streaming case. <br>`mediaStreamDescription`: The description of the stream actually switched to. This should be the same stream as what was requested by `MediaStreamDescription)`. |
| `SeekAsync(Int64)` | The `MediaElement` calls this method to ask the `MediaStreamSource` to seek to the nearest randomly accessible point before the specified time. Developers respond to this method by calling `Int64)` and by ensuring future calls to `MediaStreamSample)` will return samples from that point in the media. <br>`seekToTime`: The time as represented by 100 nanosecond increments to seek to. This is typically measured from the beginning of the media file. |
| `SetPlaybackRateAsync(Single,Boolean,Boolean)` | Requests that the `MediaStreamSource` set playback rate to the specified value. <br>`rate`: The new playback rate. <br>`thin`: Specifies whether or not to thin the stream. <br>`audio`: Specifies whether or not to enable the audio. |
| `SwitchMediaStreamAsync(Media.MediaStreamDescription)` | Called when a stream switch is requested on the `MediaElement`. <br>`mediaStreamDescription`: The stream switched to. |

## System.Windows.Media.MediaStreamSourceDiagnosticKind

Describes the type of diagnostic information used by the media.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BufferLevelInBytes` | Represents a download buffer in bytes. |
| `BufferLevelInMilliseconds` | Represents a download buffer in milliseconds. |

## System.Windows.Media.MediaStreamType

Enumeration that specifies the type of stream.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Audio` | The stream is an audio stream. |
| `Script` | The stream is a script stream. Note: Currently script commands are not supported in `MediaStreamSource`. |
| `Video` | The stream is a video stream. |

## System.Windows.Media.MultiScaleTileSource

Used to specify the source of Deep Zoom images.

### Properties / fields

| Member | Description |
|---|---|
| `TileBlendTime` | Gets or sets the amount of time to blend a new level of detail when a tile becomes available. <br>Returns: The amount of time to blend a new level of detail when a tile becomes available. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Int32,Int32,Int32,Int32,Int32)` | Creates a new instance of the `MultiScaleTileSource` class with the specified parameters. <br>`imageWidth`: The width of the Deep Zoom image. <br>`imageHeight`: The height of the Deep Zoom image. <br>`tileWidth`: The width of the tiles in the Deep Zoom image. <br>`tileHeight`: The height of the tiles in the Deep Zoom image. <br>`tileOverlap`: How much the tiles in the Deep Zoom image overlap. |
| `Constructor(Int64,Int64,Int32,Int32,Int32)` | Creates a new instance of the `MultiScaleTileSource` class with the specified parameters. <br>`imageWidth`: The width of the Deep Zoom image. <br>`imageHeight`: The height of the Deep Zoom image. <br>`tileWidth`: The width of the tiles in the Deep Zoom image. <br>`tileHeight`: The height of the tiles in the Deep Zoom image. <br>`tileOverlap`: How much the tiles in the Deep Zoom image overlap. |
| `GetTileLayers(Int32,Int32,Int32,IList{Object})` | Gets a collection of the URIs that comprise the Deep Zoom image. <br>`tileLevel`: Level of the tile. <br>`tilePositionX`: X-coordinate position of the tile. <br>`tilePositionY`: Y-coordinate position of the tile. <br>`tileImageLayerSources`: Source of the tile image layer, which is a collection of URIs. |
| `InvalidateTileLayer(Int32,Int32,Int32,Int32)` | Invalidates specified tile layers. <br>`level`: Tile level. <br>`tilePositionX`: X position of the tile. <br>`tilePositionY`: Y position of the tile. <br>`tileLayer`: Layer of the tile. |

## System.Windows.Media.PathFigure

Represents a subsection of a geometry, a single connected series of two-dimensional geometric segments.

### Properties / fields

| Member | Description |
|---|---|
| `IsClosed` | Gets or sets a value that indicates whether this figure's first and last segments are connected. <br>Returns: true if the first and last segments of the figure are connected; otherwise, false. The default is false. |
| `IsFilled` | Gets or sets a value that indicates whether the contained area of this `PathFigure` is to be used for hit-testing, rendering, and clipping. <br>Returns: true if the contained area of this `PathFigure` is to be used for hit-testing, rendering, and clipping; otherwise, false. The default is true. |
| `Segments` | Gets or sets the collection of segments that define the shape of this `PathFigure` object. <br>Returns: The collection of segments that define the shape of this `PathFigure` object. The default is an empty collection. |
| `StartPoint` | Gets or sets the `Point` where the `PathFigure` begins. <br>Returns: The `Point` where the `PathFigure` begins. The default is a `Point` with value 0,0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `IsClosedProperty` | Identifies the `IsClosed` dependency property. <br>Returns: The identifier for the `IsClosed` dependency property. |
| `IsFilledProperty` | Identifies the `IsFilled` dependency property. <br>Returns: The identifier for the `IsFilled` dependency property. |
| `SegmentsProperty` | Identifies the `Segments` dependency property. <br>Returns: The identifier for the `Segments` dependency property. |
| `StartPointProperty` | Identifies the `StartPoint` dependency property. <br>Returns: The identifier for the `StartPoint` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PathFigure` class. |

## System.Windows.Media.PathFigureCollection

Represents a collection of `PathFigure` objects that collectively make up the geometry of a `PathGeometry`.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PathFigureCollection` class. |

## System.Windows.Media.PathGeometry

Represents a complex shape that may be composed of arcs, curves, ellipses, lines, and rectangles.

### Properties / fields

| Member | Description |
|---|---|
| `Figures` | Gets or sets the collection of `PathFigure` objects that describe the contents of a path. <br>Returns: A collection of `PathFigure` objects that describe the contents of a path. Each individual `PathFigure` describes a shape. |
| `FillRule` | Gets or sets a value that determines how the intersecting areas contained in the `PathGeometry` are combined. <br>Returns: A `FillRule` enumeration value that indicates how the intersecting areas of the `PathGeometry` are combined. The default is `EvenOdd`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FiguresProperty` | Identifies the `Figures` dependency property. <br>Returns: The identifier for the `Figures` dependency property. |
| `FillRuleProperty` | Identifies the `FillRule` dependency property. <br>Returns: The identifier for the `FillRule` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PathGeometry` class. |
| `ToString` | Returns a string representation of this `PathGeometry`. <br>Returns: A string representation of this `PathGeometry`. |

## System.Windows.Media.PathSegment

Represents a segment of a `PathFigure` object.

## System.Windows.Media.PathSegmentCollection

Represents a collection of `PathSegment` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PathSegmentCollection` class. |

## System.Windows.Media.PenLineCap

Describes the shape at the end of a line or segment.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Flat` | A cap that does not extend past the last point of the line. Comparable to no line cap. |
| `Round` | A semicircle that has a diameter equal to the line thickness. |
| `Square` | A rectangle that has a height equal to the line thickness and a length equal to half the line thickness. |
| `Triangle` | An isosceles right triangle whose base length is equal to the thickness of the line. |

## System.Windows.Media.PenLineJoin

Describes the shape that joins two lines or segments.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Bevel` | Line joins use beveled vertices. |
| `Miter` | Line joins use regular angular vertices. |
| `Round` | Line joins use rounded vertices. |

## System.Windows.Media.PixelFormatType

Describes format information that reports the graphics format of individual pixels of the video format.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Format32bppArgb` | The format uses 32 bits of color information per pixel and reports color information by using alpha, red, blue, and green channels. |
| `Format8bppGrayscale` | The format uses 8 bits per pixel grey scale colors. |
| `Unknown` | The format is unknown. |

## System.Windows.Media.PlaneProjection

Represents a perspective transform (a 3-D-like effect) on an object.

### Properties / fields

| Member | Description |
|---|---|
| `CenterOfRotationX` | Gets or sets the x-coordinate of the center of rotation of the object you rotate. <br>Returns: The x-coordinate of the center of rotation of the object you rotate. Typical values are between 0 and 1 with a value of 0 corresponding to one edge of the object and 1 to the opposite edge. Values outside this range are allowed and move the center of rotation accordingly. The default is 0.5 (the center of object). |
| `CenterOfRotationY` | Gets or sets the y-coordinate of the center of rotation of the object you rotate. <br>Returns: The y-coordinate of the center of rotation of the object you rotate. Typical values are between 0 and 1 with a value of 0 corresponding to one edge of the object and 1 to the opposite edge. Values outside this range are allowed and move the center of rotation accordingly. The default is 0.5 (the center of object). |
| `CenterOfRotationZ` | Gets or sets the z-coordinate of the center of rotation of the object you rotate. <br>Returns: The z-coordinate of the center of rotation of the object you rotate. The default is 0. Values greater than 0 correspond to coordinates out from the plane of the object, and negative values correspond to coordinates behind the plane of the object. |
| `GlobalOffsetX` | Gets or sets the distance the object is translated along the x-axis of the screen. <br>Returns: The distance the object is translated along the x-axis of the screen. |
| `GlobalOffsetY` | Gets or sets the distance the object is translated along the y-axis of the screen. <br>Returns: The distance the object is translated along the y-axis of the screen. |
| `GlobalOffsetZ` | Gets or sets the distance the object is translated along the z-axis of the screen. <br>Returns: The distance the object is translated along the z-axis of the screen. |
| `LocalOffsetX` | Gets or sets the distance the object is translated along the x-axis of the plane of the object. <br>Returns: The distance the object is translated along the x-axis of the plane of the object. |
| `LocalOffsetY` | Gets or sets the distance the object is translated along the y-axis of the plane of the object. <br>Returns: The distance the object is translated along the y-axis of the plane of the object. |
| `LocalOffsetZ` | Gets or sets the distance the object is translated along the z-axis of the plane of the object. <br>Returns: The distance the object is translated along the z-axis of the plane of the object. |
| `ProjectionMatrix` | Gets the projection matrix on the `PlaneProjection`. <br>Returns: The projection matrix of the `PlaneProjection`. The default value is null. |
| `RotationX` | Gets or sets the number of degrees to rotate the object around the x-axis of rotation. <br>Returns: The number of degrees to rotate the object around the x-axis of rotation. The default is 0. |
| `RotationY` | Gets or sets the number of degrees to rotate the object around the y-axis of rotation. <br>Returns: The number of degrees to rotate the object around the y-axis of rotation. The default is 0. |
| `RotationZ` | Gets or sets the number of degrees to rotate the object around the z-axis of rotation. <br>Returns: The number of degrees to rotate the object around the z-axis of rotation. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterOfRotationXProperty` | Identifies the `CenterOfRotationX` dependency property. <br>Returns: The identifier for the `CenterOfRotationX` dependency property. |
| `CenterOfRotationYProperty` | Identifies the `CenterOfRotationY` dependency property. <br>Returns: The identifier for the `CenterOfRotationY` dependency property. |
| `CenterOfRotationZProperty` | Identifies the `CenterOfRotationZ` dependency property. <br>Returns: The identifier for the `CenterOfRotationZ` dependency property. |
| `GlobalOffsetXProperty` | Identifies the `GlobalOffsetX` dependency property. <br>Returns: The identifier for the `GlobalOffsetX` dependency property. |
| `GlobalOffsetYProperty` | Identifies the `GlobalOffsetY` dependency property. <br>Returns: The identifier for the `GlobalOffsetY` dependency property. |
| `GlobalOffsetZProperty` | Identifies the `GlobalOffsetZ` dependency property. <br>Returns: The identifier for the `GlobalOffsetZ` dependency property. |
| `LocalOffsetXProperty` | Identifies the `LocalOffsetX` dependency property. <br>Returns: The identifier for the `LocalOffsetX` dependency property. |
| `LocalOffsetYProperty` | Identifies the `LocalOffsetY` dependency property. <br>Returns: The identifier for the `LocalOffsetY` dependency property. |
| `LocalOffsetZProperty` | Identifies the `LocalOffsetZ` dependency property. <br>Returns: The identifier for the `LocalOffsetZ` dependency property. |
| `ProjectionMatrixProperty` | Identifies the `ProjectionMatrix` dependency property. <br>Returns: The identifier for the `ProjectionMatrix` dependency property. |
| `RotationXProperty` | Identifies the `RotationX` dependency property. <br>Returns: The identifier for the `RotationX` dependency property. |
| `RotationYProperty` | Identifies the `RotationY` dependency property. <br>Returns: The identifier for the `RotationY` dependency property. |
| `RotationZProperty` | Identifies the `RotationZ` dependency property. <br>Returns: The identifier for the `RotationZ` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PlaneProjection` class. |

## System.Windows.Media.PointCollection

Represents a collection of `Point` values that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PointCollection` class. |

## System.Windows.Media.PolyBezierSegment

Represents one or more cubic Bezier curves.

### Properties / fields

| Member | Description |
|---|---|
| `Points` | Gets or sets the `PointCollection` that defines this `PolyBezierSegment` object. <br>Returns: The collection of points that defines this `PolyBezierSegment` object. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PointsProperty` | Identifies the `Points` dependency property. <br>Returns: The identifier for the `Points` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PolyBezierSegment` class. |

## System.Windows.Media.PolyLineSegment

Represents a set of line segments defined by a `PointCollection` with each `Point` specifying the end point of a line segment.

### Properties / fields

| Member | Description |
|---|---|
| `Points` | Gets or sets the collection of `Point` values that defines this `PolyLineSegment` object. <br>Returns: The points that define this `PolyLineSegment` object. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PointsProperty` | Identifies the `Points` dependency property. <br>Returns: The identifier for the `Points` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PolyLineSegment` class. |

## System.Windows.Media.PolyQuadraticBezierSegment

Represents a set of quadratic Bezier segments.

### Properties / fields

| Member | Description |
|---|---|
| `Points` | Gets or sets the `PointCollection` that defines this `PolyQuadraticBezierSegment` object. <br>Returns: A collection of points that defines the shape of this `PolyQuadraticBezierSegment` object. The default value is an empty collection. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PointsProperty` | Identifies the `Points` dependency property. <br>Returns: The identifier for the `Points` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PolyQuadraticBezierSegment` class. |

## System.Windows.Media.Projection

Provides a base class for projections, which describe how to transform an object in 3-D space using perspective transforms.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Projection` class. |

## System.Windows.Media.QuadraticBezierSegment

Creates a quadratic Bezier curve between two points in a `PathFigure`.

### Properties / fields

| Member | Description |
|---|---|
| `Point1` | Gets or sets the control point of the curve. <br>Returns: The control point of this `QuadraticBezierSegment`. The default value is a `Point` with value 0,0. |
| `Point2` | Gets or sets the end `Point` of this `QuadraticBezierSegment`. <br>Returns: The end point of this `QuadraticBezierSegment`. The default value is a `Point` with value 0,0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Point1Property` | Identifies the `Point1` dependency property. <br>Returns: The identifier for the `Point1` dependency property. |
| `Point2Property` | Identifies the `Point2` dependency property. <br>Returns: The identifier for the `Point2` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `QuadraticBezierSegment` class. |

## System.Windows.Media.RadialGradientBrush

Paints an area with a radial gradient. A focal point defines the beginning of the gradient, and a circle defines the end point of the gradient.

### Properties / fields

| Member | Description |
|---|---|
| `Center` | Gets or sets the center of the outer circle of the radial gradient. <br>Returns: The two-dimensional point located at the center of the radial gradient. The default value is a `Point` with value 0.5,0.5. |
| `GradientOrigin` | Gets or sets the location of the focal point that defines the beginning of the gradient. <br>Returns: The two-dimensional point located at the center of the radial gradient. The default value is a `Point` with value 0.5,0.5. |
| `RadiusX` | Gets or sets the horizontal radius of the outer circle of the radial gradient. <br>Returns: The horizontal radius of the outermost circle of the radial gradient. The default value is 0.5. |
| `RadiusY` | Gets or sets the vertical radius of the outer circle of a radial gradient. <br>Returns: The vertical radius of the outer circle of a radial gradient. The default is 0.5. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterProperty` | Identifies the `Center` dependency property. <br>Returns: The identifier for the `Center` dependency property. |
| `GradientOriginProperty` | Identifies the `GradientOrigin` dependency property. <br>Returns: The identifier for the `GradientOrigin` dependency property. |
| `RadiusXProperty` | Identifies the `RadiusX` dependency property. <br>Returns: The identifier for the `RadiusX` dependency property. |
| `RadiusYProperty` | Identifies the `RadiusY` dependency property. <br>Returns: The identifier for the `RadiusY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `RadialGradientBrush` class. |
| `Constructor(Media.Color,Media.Color)` | Initializes a new instance of the `RadialGradientBrush` class with the specified start and stop colors. <br>`startColor`: Color value at the focus (`GradientOrigin`) of the radial gradient. <br>`endColor`: Color value at the outer edge of the radial gradient. |
| `Constructor(Media.GradientStopCollection)` | Initializes a new instance of the `RadialGradientBrush` class that has the specified gradient stops. <br>`gradientStopCollection`: The gradient stops to set on this brush. |

## System.Windows.Media.RateChangedRoutedEventArgs

Provides data for the `RateChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `NewRate` | Gets the new rate <br>Returns: The new rate. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instances of the `RateChangedRoutedEventArgs` class. |

## System.Windows.Media.RateChangedRoutedEventHandler

Represents the method that will handle the `RateChanged`.

## System.Windows.Media.RectangleGeometry

Describes a two-dimensional rectangular geometry.

### Properties / fields

| Member | Description |
|---|---|
| `RadiusX` | Gets or sets the x-radius of the ellipse that is used to round the corners of the rectangle. <br>Returns: The x-radius of the ellipse used to round the corners of the rectangle geometry. The default is 0. |
| `RadiusY` | Gets or sets the y-radius of the ellipse that is used to round the corners of the rectangle. <br>Returns: The y-radius of the ellipse used to round the corners of the rectangle geometry. The default is 0. |
| `Rect` | Gets or sets the dimensions of the rectangle. <br>Returns: The `Rect` structure that describes the position and size of the rectangle. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `RadiusXProperty` | Identifies the `RadiusX` dependency property. <br>Returns: The identifier for the `RadiusX` dependency property. |
| `RadiusYProperty` | Identifies the `RadiusY` dependency property. <br>Returns: The identifier for the `RadiusY` dependency property. |
| `RectProperty` | Identifies the `Rect` dependency property. <br>Returns: The identifier for the `Rect` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `RectangleGeometry` class, and creates a rectangle with zero area. |

## System.Windows.Media.RenderingEventArgs

Provides event data for the `Rendering` event.

### Properties / fields

| Member | Description |
|---|---|
| `RenderingTime` | Gets a date/time when the frame rendered. <br>Returns: The date/time when the frame rendered. |

## System.Windows.Media.RotateTransform

Rotates an object clockwise about a specified point in a two-dimensional x-y coordinate system.

### Properties / fields

| Member | Description |
|---|---|
| `Angle` | Gets or sets the angle, in degrees, of clockwise rotation. <br>Returns: The angle, in degrees, of clockwise rotation. The default is 0. |
| `CenterX` | Gets or sets the x-coordinate of the rotation center point. <br>Returns: The x-coordinate of the center of rotation. The default is 0. |
| `CenterY` | Gets or sets the y-coordinate of the rotation center point. <br>Returns: The y-coordinate of the center of rotation. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AngleProperty` | Identifies the `Angle` dependency property. <br>Returns: The identifier for the `Angle` dependency property. |
| `CenterXProperty` | Identifies the `CenterX` dependency property. <br>Returns: The identifier for the `CenterX` dependency property. |
| `CenterYProperty` | Identifies the `CenterY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `RotateTransform` class. |

## System.Windows.Media.SampleAllocationMode

Provides values for specifying how the camera video buffer is maintained on Silverlight for Windows Phone.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `NewBuffer` | A new buffer is created for each sample captured. This is the same behavior on Silverlight for the desktop. |
| `ReuseBuffer` | A new buffer is created for each sample capture. This can improve performance on Silverlight for Windows Phone by reducing the amount of garbage collection that is required. |

## System.Windows.Media.ScaleTransform

Scales an object in the two-dimensional x-y coordinate system.

### Properties / fields

| Member | Description |
|---|---|
| `CenterX` | Gets or sets the x-coordinate of the center point of this `ScaleTransform`. <br>Returns: The x-coordinate of the center point of this `ScaleTransform`. The default is 0. |
| `CenterY` | Gets or sets the y-coordinate of the center point of this `ScaleTransform`. <br>Returns: The y-coordinate of the center point of this `ScaleTransform`. The default is 0. |
| `ScaleX` | Gets or sets the x-axis scale factor. <br>Returns: The scale factor along the x-axis. The default is 1. |
| `ScaleY` | Gets or sets the y-axis scale factor. <br>Returns: The scale factor along the y-axis. The default is 1. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterXProperty` | Identifies the `CenterX` dependency property. <br>Returns: The identifier for the `CenterX` dependency property. |
| `CenterYProperty` | Identifies the `CenterY` dependency property. <br>Returns: The identifier for the `CenterY` dependency property. |
| `ScaleXProperty` | Identifies the `ScaleX` dependency property. <br>Returns: The identifier for the `ScaleX` dependency property. |
| `ScaleYProperty` | Identifies the `ScaleY` dependency property. <br>Returns: The identifier for the `ScaleY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ScaleTransform` class. |

## System.Windows.Media.SkewTransform

Represents a two-dimensional skew.

### Properties / fields

| Member | Description |
|---|---|
| `AngleX` | Gets or sets the x-axis skew angle, which is measured in degrees counterclockwise from the y-axis. <br>Returns: The skew angle, which is measured in degrees counterclockwise from the y-axis. The default is 0. |
| `AngleY` | Gets or sets the y-axis skew angle, which is measured in degrees counterclockwise from the x-axis. <br>Returns: The skew angle, which is measured in degrees counterclockwise from the x-axis. The default is 0. |
| `CenterX` | Gets or sets the x-coordinate of the transform center. <br>Returns: The x-coordinate of the transform center. The default is 0. |
| `CenterY` | Gets or sets the y-coordinate of the transform center. <br>Returns: The y-coordinate of the transform center. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AngleXProperty` | Identifies the `AngleX` dependency property. <br>Returns: The identifier for the `AngleX` dependency property. |
| `AngleYProperty` | Identifies the `AngleY` dependency property. <br>Returns: The identifier for the `AngleY` dependency property. |
| `CenterXProperty` | Identifies the `CenterX` dependency property. <br>Returns: The identifier for the `CenterX` dependency property. |
| `CenterYProperty` | Identifies the `CenterY` dependency property. <br>Returns: The identifier for the `CenterY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SkewTransform` class. |

## System.Windows.Media.SolidColorBrush

Paints an area with a solid color.

### Properties / fields

| Member | Description |
|---|---|
| `Color` | Gets or sets the color of this `SolidColorBrush`. <br>Returns: The brush's color. The default value is `Transparent`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ColorProperty` | Identifies the `Color` dependency property. <br>Returns: The identifier for the `Color` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SolidColorBrush` class with no color. |
| `Constructor(Media.Color)` | Initializes a new instance of the `SolidColorBrush` class with the specified `Color`. <br>`color`: The color to apply to the brush. |

## System.Windows.Media.Stretch

Describes how content is resized to fill its allocated space.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Fill` | The content is resized to fill the destination dimensions. The aspect ratio is not preserved. |
| `None` | The content preserves its original size. |
| `Uniform` | The content is resized to fit in the destination dimensions while it preserves its native aspect ratio. |
| `UniformToFill` | The content is resized to fill the destination dimensions while it preserves its native aspect ratio. If the aspect ratio of the destination rectangle differs from the source, the source content is clipped to fit in the destination dimensions. |

## System.Windows.Media.StyleSimulations

Describes the simulation style of a font.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BoldItalicSimulation` | Bold and Italic style simulation. |
| `BoldSimulation` | Bold style simulation. |
| `ItalicSimulation` | Italic style simulation. |
| `None` | No font style simulation. |

## System.Windows.Media.SweepDirection

Specifies the direction an elliptical arc is drawn.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Clockwise` | Arcs are drawn in a clockwise (positive-angle) direction. |
| `Counterclockwise` | Aarcs are drawn in a counterclockwise (negative-angle) direction. |

## System.Windows.Media.TextHintingMode

Specifies whether text rendering is optimized for readability or for dynamic presentation.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Animated` | Turns off text rendering optimizations. |
| `Fixed` | The default text render mode. |

## System.Windows.Media.TextOptions

Provides options for controlling the rendering behavior of text, which can be set through an attached property usage on a wide range of Silverlight types.

### Properties / fields

| Member | Description |
|---|---|
| `TextHintingMode` | Gets or sets a value that indicates whether text rendering is optimized for readability or animation. <br>Returns: A value that indicates whether text rendering is optimized for readability or animation. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `TextHintingModeProperty` | Identifies the `TextHintingMode` attached property. <br>Returns: The identifier for the `TextHintingMode` attached property. |

### Methods

| Member | Description |
|---|---|
| `GetTextHintingMode(FrameworkElement)` | Gets the `TextHintingMode` for this `FrameworkElement`. <br>`target`: The `FrameworkElement` to get this property on. <br>Returns: The `TextHintingMode` for this `FrameworkElement`. |
| `SetTextHintingMode(FrameworkElement,Media.TextHintingMode)` | Sets the `TextHintingMode` for this `FrameworkElement`. <br>`target`: The `FrameworkElement` to set this property on. <br>`textHintingMode`: The `TextHintingMode` to set the `FrameworkElement` to. |

## System.Windows.Media.TileBrush

Base class that describes a way to paint a region.

### Properties / fields

| Member | Description |
|---|---|
| `AlignmentX` | Gets or sets the horizontal alignment of content in the `TileBrush` base tile. <br>Returns: A value that specifies the horizontal position of `TileBrush` content in its base tile. The default value is `Center`. |
| `AlignmentY` | Gets or sets the vertical alignment of content in the `TileBrush` base tile. <br>Returns: A value that specifies the vertical position of `TileBrush` content in its base tile. The default value is `Center`. |
| `Stretch` | Gets or sets a value that specifies how the content of this `TileBrush` stretches to fit its tiles. <br>Returns: A value that specifies how this `TileBrush` content is projected onto its base tile. The default value is `Fill`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AlignmentXProperty` | Identifies the `AlignmentX` dependency property. <br>Returns: The identifier for the `AlignmentX` dependency property. |
| `AlignmentYProperty` | Identifies the `AlignmentY` dependency property. <br>Returns: The identifier for the `AlignmentY` dependency property. |
| `StretchProperty` | Identifies the `Stretch` dependency property. <br>Returns: The identifier for the `Stretch` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Provides initialization for base class values when called by the constructor of a derived class. |

## System.Windows.Media.TimelineMarker

Represents metadata associated with a specific point in a media file.

### Properties / fields

| Member | Description |
|---|---|
| `Text` | Gets or sets the text value of a `TimelineMarker`. <br>Returns: The text value of the `TimelineMarker`. The default value is an empty string. |
| `Time` | Gets or sets the time at which a `TimelineMarker` is reached. <br>Returns: The time at which the TimelineMarker is reached. The default value is null. |
| `Type` | Gets or sets the marker type of a `TimelineMarker`. <br>Returns: A string that describes the type of this TimelineMarker. The default value is an empty string. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `TextProperty` | Identifies the `Text` dependency property. <br>Returns: The identifier for the `Text` dependency property. |
| `TimeProperty` | Identifies the `Time` dependency property. <br>Returns: The identifier for the `Time` dependency property. |
| `TypeProperty` | Identifies the `Type` dependency property. <br>Returns: The identifier for the `Type` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TimelineMarker` class. |

## System.Windows.Media.TimelineMarkerCollection

Represents a collection of `TimelineMarker` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TimelineMarkerCollection` class. |

## System.Windows.Media.TimelineMarkerRoutedEventArgs

Provides event data for the `MarkerReached` event.

### Properties / fields

| Member | Description |
|---|---|
| `Marker` | Gets the `TimelineMarker` that triggered this event. <br>Returns: The `TimelineMarker` that triggered this event. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TimelineMarkerRoutedEventArgs` class. |

## System.Windows.Media.TimelineMarkerRoutedEventHandler

Represents methods that will handle various routed events related to timeline markers.

## System.Windows.Media.Transform

Defines functionality that enables transformations in a two-dimensional plane.

### Properties / fields

| Member | Description |
|---|---|
| `Inverse` | Gets the inverse of this transform, if it exists. <br>Returns: The inverse of this transform, if it exists; otherwise, null. |

### Methods

| Member | Description |
|---|---|
| `TransformBounds(Rect)` | Transforms the specified bounding box and returns an axis-aligned bounding box that is exactly large enough to contain it. <br>`rect`: The bounding box to transform. <br>Returns: The smallest axis-aligned bounding box that can contain the transformed . |
| `TryTransform(Point,Point@)` | Attempts to transform the specified point and returns a value that indicates whether the transformation was successful. <br>`inPoint`: The point to transform. <br>`outPoint`: The result of transforming . <br>Returns: true if was transformed; otherwise, false. |

## System.Windows.Media.TransformCollection

Represents a collection of `Transform` objects that can be individually accessed by index.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TransformCollection` class. |

## System.Windows.Media.TransformGroup

Represents a composite `Transform` composed of other `Transform` objects.

### Properties / fields

| Member | Description |
|---|---|
| `Children` | Gets or sets the collection of child `Transform` objects. <br>Returns: The collection of child `Transform` objects. The default is an empty collection. |
| `Value` | Gets the `Matrix` structure that describes the transformation represented by this `TransformGroup`. <br>Returns: A composite of the `Transform` objects in this `TransformGroup`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ChildrenProperty` | Identifies the `Children` dependency property. <br>Returns: The identifier for the `Children` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TransformGroup` class. |

## System.Windows.Media.TranslateTransform

Translates (moves) an object in the two-dimensional x-y coordinate system.

### Properties / fields

| Member | Description |
|---|---|
| `X` | Gets or sets the distance to translate along the x-axis. <br>Returns: The distance to translate (move) an object along the x-axis, in pixels. This property is read/write. The default is 0. |
| `Y` | Gets or sets the distance to translate (move) an object along the y-axis. <br>Returns: The distance to translate (move) an object along the y-axis, in pixels. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `XProperty` | Identifies the `X` dependency property. <br>Returns: The identifier for the `X` dependency property. |
| `YProperty` | Identifies the `Y` dependency property. <br>Returns: The identifier for the `Y` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TranslateTransform` class. |

## System.Windows.Media.Typeface

Represents a combination of the `FontFamily`, `FontWeight`, `FontStyle`, and `FontStretch` classes.

### Methods

| Member | Description |
|---|---|
| `TryGetGlyphTypeface(Media.GlyphTypeface@)` | Retrieves the `GlyphTypeface` that corresponds to the `Typeface`. <br>`glyphTypeface`: A `GlyphTypeface` object that corresponds to this typeface, or a null reference (Nothing in Visual Basic) if the typeface was constructed from a composite font. <br>Returns: true if the out parameter is set to a `GlyphTypeface` object; otherwise, false. |

## System.Windows.Media.Typography

Not Implemented.

## System.Windows.Media.VideoBrush

Paints an area with video content.

### Properties / fields

| Member | Description |
|---|---|
| `SourceName` | Gets or sets the name of the `MediaElement` to use as the source of the `VideoBrush`. <br>Returns: The name of the `MediaElement` to use as the source of the `VideoBrush`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `SourceNameProperty` | Identifies the `SourceName` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VideoBrush` class. |
| `SetSource(MediaElement)` | Sets the source of the `VideoBrush` using a media file source from an intermediary `MediaElement` control. <br>`source`: The intermediary `MediaElement` control used as the source for the `VideoBrush`. |
| `SetSource(Media.CaptureSource)` | Sets the source of the `VideoBrush` using a video capture source <br>`source`: A `CaptureSource` as obtained from video capture out of a `VideoSink`. |

## System.Windows.Media.VideoCaptureDevice

Describes desired and supported video format information for a video capture device, such as a webcam.

### Properties / fields

| Member | Description |
|---|---|
| `DesiredFormat` | Gets or sets the desired video format to be used by this video capture device. <br>Returns: The desired video format to be used by this video capture device, as a `VideoFormat` object. The default is null.On Mac OSX, the `SupportedFormats` is always empty. To set the `DesiredFormat`, you can insatiate a new `VideoFormat` object with a `PixelFormat` of `Unknown`. |
| `SupportedFormats` | Gets a generic collection of supported video formats for this video capture device. <br>Returns: A generic collection of `VideoFormat` objects that represents the supported video formats for this video capture device.On Mac OSX, the `SupportedFormats` is always empty. To set the `DesiredFormat`, you can insatiate a new `VideoFormat` object with a `PixelFormat` of `Unknown`. |

## System.Windows.Media.VideoCaptureDeviceCollection

Specifies a collection of `VideoCaptureDevice` objects.

## System.Windows.Media.VideoFormat

Provides specific technical information about a video format that is used by a video capture device.

### Properties / fields

| Member | Description |
|---|---|
| `FramesPerSecond` | Gets the number of frames per second for the represented video format. <br>Returns: The number of frames per second for the represented video format. |
| `PixelFormat` | Gets the graphics format information for individual pixels of the video format. <br>Returns: A `PixelFormatType` value that reports the format of individual pixels of the video format. |
| `PixelHeight` | Gets the height of the camera-framing area for the represented video format. <br>Returns: The height, in pixels, of the camera-framing area for the represented video format. |
| `PixelWidth` | Gets the width of the camera-framing area for the represented video format. <br>Returns: The width, in pixels, of the camera-framing area for the represented video format. |
| `Stride` | Gets the array stride that is used in the video format. <br>Returns: The array stride that is used in the video format. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Media.PixelFormatType,Int32,Int32,Single)` | Initializes a new instance of the `VideoFormat` class. <br>`pixelFormat`: The initial `PixelFormat` value. <br>`width`: The initial `PixelWidth` value. <br>`height`: The initial `PixelHeight` value. <br>`framesPerSecond`: The initial `FramesPerSecond` value. |

## System.Windows.Media.VideoOutputConnector

Provides the actual data about connector types and specifies the output protection schemes in which Silverlight can engage.

### Properties / fields

| Member | Description |
|---|---|
| `CanEnableCgmsa` | Gets a value that indicates whether this connector supports the Copy Generation Management System—Analog, which is an analog copy management system. <br>Returns: A value that indicates whether this connector supports the Copy Generation Management System—Analog, which is an analog copy management system. The default value is false. |
| `CanEnableHdcp` | Gets a value that indicates whether this connector can engage the High-Bandwidth Digital Content Protection output protection scheme. <br>Returns: A value that indicates whether this connector can engage the High-Bandwidth Digital Content Protection output protection scheme. The default value is false. |
| `ConnectorType` | Gets the connector type with which the output device is connected to the graphics card. <br>Returns: The connector type with which the output device is connected to the graphics card. The default value is `Other`. |

## System.Windows.Media.VideoOutputConnectorType

Defines all the output connector types that are typically available.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ComponentVideo` | Component video connector. |
| `CompositeVideo` | Composite video connector. |
| `DisplayPortExternal` | External DisplayPort connector. |
| `DisplayPortInternal` | Embedded DisplayPort connector. |
| `Dvi` | Digital video interface (DVI) connector. |
| `Hdmi` | High-definition multimedia interface (HDMI) connector. |
| `Internal` | Internal connector. The connection between the graphics adapter and the display device is permanent and not available to the user. |
| `JapaneseDConnector` | Japanese D connector, a connector that conforms to the Electronic Industries Association of Japan (EIAJ) RC-5237 standard. |
| `Llvds` | Low-voltage differential signaling (LVDS) connector. |
| `Other` | An unknown connector type. |
| `SVideo` | S-Video connector. |
| `Sdi` | SDI (serial digital image) connector. |
| `Tmds` | TMDS connector. |
| `UdiExternal` | External Unified Display Interface (UDI). |
| `UdiInternal` | Embedded Unified Display Interface (UDI). |
| `Vga` | Video graphics array (VGA) connector. |

## System.Windows.Media.VideoSink

Exposes the capture graph for video devices. Derive from this class to receive video information and to obtain the capture graph through `CaptureSource`.

### Properties / fields

| Member | Description |
|---|---|
| `AllocationMode` | Specifies how the camera video buffer is maintained on Silverlight for Windows Phone. <br>Returns: The sample allocation mode. |
| `CaptureSource` | Gets or sets a capture source that is associated with this `VideoSink`. <br>Returns: A capture source that is associated with this `VideoSink`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VideoSink` class. |
| `Finalize` | Frees resources that are used by the `VideoSink` class. |
| `OnCaptureStarted` | When overridden in a derived class, is invoked when a video device starts capturing video data. |
| `OnCaptureStopped` | When overridden in a derived class, is invoked when a video device stops capturing video data. |
| `OnFormatChange(Media.VideoFormat)` | When overridden in a derived class, is invoked when a video device reports a video format change. <br>`videoFormat`: The new video format. |
| `OnSample(Int64,Int64,Byte[])` | When overridden in a derived class, is invoked when a video device captures a complete video sample. <br>`sampleTimeInHundredNanoseconds`: The time, in 100-nanosecond units, when the sample was captured. <br>`frameDurationInHundredNanoseconds`: The duration of the sample, in 100-nanosecond units. <br>`sampleData`: A byte stream that contains video data. The byte stream should be processed into samples according to the relevant video format information. |

## System.Windows.Media.VisualTreeHelper

Provides utility methods that can used to traverse object relationships (along child object or parent object axes) in the Silverlight visual tree.

### Methods

| Member | Description |
|---|---|
| `FindElementsInHostCoordinates(Point,UIElement)` | Retrieves a set of objects that are located within a specified point of an object's coordinate space. <br>`intersectingPoint`: The point to use as the determination point. <br>`subtree`: The object to search within. <br>Returns: An enumerable set of `UIElement` objects that are determined to be located in the visual tree composition at the specified point and within the specified subtee. |
| `FindElementsInHostCoordinates(Rect,UIElement)` | Retrieves a set of objects that are located within a specified `Rect` of an object's coordinate space. <br>`intersectingRect`: The `Rect` to use as the determination area. <br>`subtree`: The object to search within. <br>Returns: An enumerable set of `UIElement` objects that are determined to be located in the visual tree composition at the specified point and within the specified subtee. |
| `GetChild(DependencyObject,Int32)` | Using the provided index, obtains a specific child object of the provided object by examining the visual tree. <br>`reference`: The object that holds the child collection. <br>`childIndex`: The index of the requested child object in the child collection. <br>Returns: The child object as referenced by . |
| `GetChildrenCount(DependencyObject)` | Returns the number of children that exists in an object's child collection in the visual tree. <br>`reference`: The source visual. <br>Returns: The number of visual children for the provided source visual. |
| `GetOpenPopups` | Gets all open `Popup` controls. <br>Returns: An `IEnumerable`1` of all open `Popup` controls. |
| `GetParent(DependencyObject)` | Returns an object's parent object in the visual tree. <br>`reference`: The object to get the parent object for. <br>Returns: The parent object of the object in the visual tree. |
| `GetRoot(DependencyObject)` | Returns an object's root object in the visual tree. <br>`reference`: The object to get the root object for. <br>Returns: The root object of the object in the visual tree. |

## System.Windows.Media.WaveFormatType

Reports the digital signal encoding format of an audio format.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Pcm` | Audio format uses pulse code modulation (PCM) encoding. |

## System.Windows.MediaFailedRoutedEventArgs

Provides event data for media failed events.

### Properties / fields

| Member | Description |
|---|---|
| `ErrorTrace` | Gets the trace information for a media failed event. <br>Returns: The error trace for the failed media event. |

## System.Windows.MessageBox

Displays a message to the user and optionally prompts for a response.

### Methods

| Member | Description |
|---|---|
| `Show(String)` | Displays a message box that contains the specified text and an OK button. <br>`messageBoxText`: The message to display. <br>Returns: `OK` in all cases. |
| `Show(String,String,MessageBoxButton)` | Displays a message box that contains the specified text, title bar caption, and response buttons. <br>`messageBoxText`: The message to display. <br>`caption`: The title of the message box. <br>`button`: A value that indicates the button or buttons to display. <br>Returns: A value that indicates the user's response to the message. |

## System.Windows.MessageBoxButton

Specifies the buttons to include when you display a message box.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `OK` | Displays only the OK button. |
| `OKCancel` | Displays both the OK and Cancel buttons. |

## System.Windows.MessageBoxResult

Represents a user's response to a message box.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Cancel` | The user clicked the Cancel button or pressed ESC. |
| `No` | This value is not currently used. |
| `None` | This value is not currently used. |
| `OK` | The user clicked the OK button. |
| `Yes` | This value is not currently used. |

## System.Windows.Messaging.ListenFailedException

Represents an exception that occurs when a `LocalMessageReceiver` cannot be registered.

## System.Windows.Messaging.LocalMessageReceiver

Represents the receiving end of a local messaging channel between two Silverlight-based applications.

### Properties / fields

| Member | Description |
|---|---|
| `AllowedSenderDomains` | Gets the domains that the receiver can receive messages from. <br>Returns: The domains that the receiver can receive messages from, or `AnyDomain` if the receiver can receive messages from any domain. |
| `DisableSenderTrustCheck` | Gets or sets a value that indicates whether the receiver can receive messages from a sender with a different Protected Mode setting. <br>Returns: true if the receiver can receive messages regardless of the sender's Protected Mode setting; otherwise, false. |
| `NameScope` | Gets a value that indicates whether the `ReceiverName` is scoped to the global namescope or to the receiver's specific domain. <br>Returns: A value that indicates whether the `ReceiverName` is scoped to the global namescope or to the receiver's specific domain. |
| `ReceiverName` | Gets the name of the receiver. <br>Returns: The name of the receiver. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AnyDomain` | A value that represents any domain. |

### Events

| Member | Description |
|---|---|
| `MessageReceived` | Occurs when a message is received from a `LocalMessageSender`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `LocalMessageReceiver` class and configures it with the specified name. <br>`receiverName`: The name of the receiver. |
| `Constructor(String,Messaging.ReceiverNameScope,IEnumerable{String})` | Initializes a new instance of the `LocalMessageReceiver` class and configures it with the specified name, namescope requirement, and allowed sender domains. <br>`receiverName`: The name of the receiver, which must be unique either within the global namescope or the receiver's domain, depending on the value of the parameter. <br>`nameScope`: A value that indicates whether the is scoped to the global namescope or to the receiver's specific domain. <br>`allowedSenderDomains`: The domains that the receiver can receive messages from, or `AnyDomain` to receive from any domain. |
| `Dispose` | Stops the receiver from receiving messages and releases all resources used by the receiver. |
| `Listen` | Starts listening for messages from a `LocalMessageSender`. |

## System.Windows.Messaging.LocalMessageSender

Represents the sending end of a local messaging channel between two Silverlight-based applications.

### Properties / fields

| Member | Description |
|---|---|
| `ReceiverDomain` | Gets the domain of the `LocalMessageReceiver` that this sender will send messages to. <br>Returns: The domain of the `LocalMessageReceiver` that this sender will send messages to, depending on the receiver's `NameScope` property value. |
| `ReceiverName` | Gets the name of the `LocalMessageReceiver` that this sender will send messages to. <br>Returns: The name of the `LocalMessageReceiver` that this sender will send messages to. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Global` | A value that represents the global domain. |

### Events

| Member | Description |
|---|---|
| `SendCompleted` | Occurs when the message has been successfully sent. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `LocalMessageSender` class and configures it to send messages to the receiver with the specified name. <br>`receiverName`: The `ReceiverName` property value of the `LocalMessageReceiver` that this sender will send messages to. |
| `Constructor(String,String)` | Initializes a new instance of the `LocalMessageSender` class and configures it to send messages to the receiver with the specified name and domain. <br>`receiverName`: The `ReceiverName` property value of the `LocalMessageReceiver` that this sender will send messages to. <br>`receiverDomain`: The domain of the `LocalMessageReceiver` that this sender will send messages to, or `Global` if the receiver is not scoped to a domain. |
| `SendAsync(String)` | Sends the specified message to the configured receiver asynchronously. <br>`message`: The message to send. |
| `SendAsync(String,Object)` | Sends the specified messages to the configured receiver asynchronously. <br>`message`: The message to send. <br>`userState`: A unique user-state object that functions as a task ID for the message transfer. |

## System.Windows.Messaging.MessageReceivedEventArgs

Provides data for the `MessageReceived` event.

### Properties / fields

| Member | Description |
|---|---|
| `Message` | Gets the message sent from a `LocalMessageSender` to a `LocalMessageReceiver`. <br>Returns: The message that was sent. |
| `NameScope` | Gets a value that indicates whether the `LocalMessageReceiver` is configured to receive messages from the global namescope or from a specific domain. <br>Returns: A value that indicates whether the receiver is configured for a global or domain namescope. |
| `ReceiverName` | Gets the name of the `LocalMessageReceiver` that received the message. <br>Returns: The name of the message receiver. |
| `Response` | Gets or sets a response message to be sent to the `LocalMessageSender` that sent the original message. <br>Returns: The response message to be sent to the sender of the original message. |
| `SenderDomain` | Gets the domain of the `LocalMessageSender` that sent the message. <br>Returns: The domain of the `LocalMessageSender` that sent the message. |

## System.Windows.Messaging.ReceiverNameScope

Defines values that indicate whether a `LocalMessageReceiver` name is scoped to the global domain or to a specific domain.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Domain` | The receiver name is scoped to the domain of the receiver. |
| `Global` | The receiver name is scoped to the global domain. |

## System.Windows.Messaging.SendCompletedEventArgs

Provides data for the `SendCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `Message` | Gets the message sent from a `LocalMessageSender` to a `LocalMessageReceiver`. <br>Returns: The message that was sent. |
| `ReceiverDomain` | Gets the domain of the `LocalMessageReceiver` that received the message. <br>Returns: The domain of the `LocalMessageReceiver` that received the message. |
| `ReceiverName` | Gets the name of the `LocalMessageReceiver` that received the message. <br>Returns: The name of the message receiver. |
| `Response` | Gets the response message sent to the `LocalMessageSender` from the `LocalMessageReceiver` that received the original message. <br>Returns: The response message sent to the sender of the original message. |

## System.Windows.Messaging.SendFailedException

Represents an exception that can occur when a `LocalMessageSender` fails to send a message.

## System.Windows.Navigation.LoadCompletedEventHandler

Represents the method that will handle the `LoadCompleted` event.

## System.Windows.Navigation.NavigatedEventHandler

Represents the method that will handle the `Navigated` event.

## System.Windows.Navigation.NavigatingCancelEventArgs

Provides data for the `NavigatingCancelEventArgs)` method and the `Navigating` event.

### Properties / fields

| Member | Description |
|---|---|
| `IsCancelable` | Gets a value that indicates whether you can cancel the navigation. <br>Returns: true if you can cancel the navigation; otherwise, false. |
| `IsNavigationInitiator` | Gets a value that indicates whether the current application is the origin and destination of the navigation. <br>Returns: true if the navigation starts and ends within the current application; false if the navigation starts or ends at an external location. |
| `NavigationMode` | Gets a value that indicates the type of navigation that is occurring. <br>Returns: A value that indicates the type of navigation (`Back`, `Forward`, or `New`) that is occurring. |
| `Uri` | Gets the uniform resource identifier (URI) for the content that is being navigated to. <br>Returns: A value that represents the URI for the content. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Uri,Navigation.NavigationMode)` | Initializes a new instance of the `NavigatingCancelEventArgs` class, based on URI and mode. <br>`uri`: The uniform resource identifier (URI) for the content that is being navigated to. <br>`mode`: A value that indicates the type of navigation that is occurring. |
| `Constructor(Uri,Navigation.NavigationMode,Boolean,Boolean)` | Initializes a new instance of the `NavigatingCancelEventArgs` class, setting all initial property values. <br>`uri`: The uniform resource identifier (URI) for the content that is being navigated to. <br>`mode`: A value that indicates the type of navigation that is occurring. <br>`IsCancelable`: Sets the initial `IsCancelable` value. <br>`IsNavigationInitiator`: Sets the initial `IsNavigationInitiator` value. |

## System.Windows.Navigation.NavigatingCancelEventHandler

Represents the method that will handle the `Navigating` event, which is a cancelable event.

## System.Windows.Navigation.NavigationEventArgs

Provides data for navigation methods and event handlers that cannot cancel the navigation request.

### Properties / fields

| Member | Description |
|---|---|
| `Content` | Gets the content of the target being navigated to. <br>Returns: An object that represents the target content. |
| `IsNavigationInitiator` | Gets a value that indicates whether the current application is the origin and destination of the navigation. <br>Returns: true if the navigation starts and ends within the current application; false if the navigation starts or ends at an external location. |
| `NavigationMode` | Gets a value that indicates the type of navigation that is occurring. <br>Returns: A value that indicates the type of navigation (`Back`, `Forward`, or `New`) that is occurring. |
| `Uri` | Gets the uniform resource identifier (URI) of the target. <br>Returns: A value that represents the URI. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object,Uri)` | Initializes a new instance of the `NavigationEventArgs` class, based on content and URI. <br>`content`: Initializes the `Content` property. <br>`uri`: Initializes the `Uri` property. |
| `Constructor(Object,Uri,Navigation.NavigationMode,Boolean)` | Initializes a new instance of the `NavigationEventArgs` class, setting initial values for all properties. <br>`content`: Initializes the `Content` property. <br>`uri`: Initializes the `Uri` property. <br>`NavigationMode`: Initializes the `NavigationMode` property. <br>`IsNavigationInitiator`: Initializes the `IsNavigationInitiator` property. |

## System.Windows.Navigation.NavigationMode

Specifies the type of navigation that is occurring.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Back` | Navigating to the most recent content in the back navigation history. This value is used when the `GoBack` method is called. |
| `Forward` | Navigating to the most recent content in the forward navigation history. This value is used when the `GoForward` method is called. |
| `New` | Navigating to new content. This value is used when the `Uri)` method is called or when the `Source` property is set. It is also used for any navigation request that the user initiates from the Web browser (including the back or forward buttons in a Web browser). |
| `Refresh` | Reloading the current content. This value is used when the `Refresh` method is called. |

## System.Windows.NotificationWindow

Represents a notification area that is displayed in the system area. Notifications can only be enabled for an out-of-browser application; browser-hosted applications cannot access this notification area.

### Properties / fields

| Member | Description |
|---|---|
| `Content` | Gets or sets the root of visual elements that define the visual look of the notification. <br>Returns: A single `FrameworkElement` that includes the root of a visual tree. The visual tree defines the visual look of the notification. |
| `Height` | Gets or sets the height, in pixels, of this notification window. See Remarks. <br>Returns: The height, in pixels, of this notification window when it is displayed. |
| `Visibility` | Gets a value that determines whether this notification is currently being displayed. <br>Returns: true if the notification is currently displayed; otherwise, false. |
| `Width` | Gets or sets the width, in pixels, of this notification window. See Remarks. <br>Returns: The width, in pixels, of this notification window when it is displayed. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ContentProperty` | Identifies the `Content` dependency property. <br>Returns: The identifier for the `Content` dependency property. |
| `HeightProperty` | Identifies the `Height` dependency property. <br>Returns: The identifier for the `Height` dependency property. |
| `WidthProperty` | Identifies the `Width` dependency property. <br>Returns: The identifier for the `Width` dependency property. |

### Events

| Member | Description |
|---|---|
| `Closed` | Occurs when `Close` is called, or when the notification window times out and has finished its fadeout animation. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `NotificationWindow` class. |
| `Close` | Immediately closes the notification window. |
| `Show(Int32)` | Displays the notification window for the specified number of milliseconds before it times out. <br>`durationInMilliseconds`: The duration that the notification window should remain displayed in the system area, specified in milliseconds. |

## System.Windows.NullableBoolConverter

Converts `Nullable`1` types (using the `Boolean` type constraint on the generic) from strings.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `NullableBoolConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether this converter can convert an object of the specified type to the `Nullable`1` type (using the `Boolean` type constraint on the generic). <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: The type that you want to convert from. <br>Returns: true if `sourceType` is a `String`, `Boolean`, or a `Nullable`1` type (using the `Boolean` type constraint on the generic) that can be assigned from `sourceType`; otherwise, false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `Nullable`1` type (using the `Boolean` type constraint on the generic). <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: The `CultureInfo` to use as the current culture. <br>`value`: The object to convert to a `Nullable`1` type (using the `Boolean` type constraint on the generic). <br>Returns: A `Nullable`1` type (using the `Boolean` type constraint on the generic) that represents the converted object. |

## System.Windows.OutOfBrowserSettings

Represents information about an application that is configured for out-of-browser support.

### Properties / fields

| Member | Description |
|---|---|
| `Blurb` | Gets a short description of the application. <br>Returns: A short description of the application. |
| `EnableGPUAcceleration` | Gets a value that indicates whether to use graphics processor unit hardware acceleration for cached compositions, which potentially results in graphics optimization. <br>Returns: true if GPU acceleration is enabled; otherwise, false. The default is false. |
| `Icons` | Gets a collection of `Icon` instances associated with the application. <br>Returns: The icons associated with the application. |
| `SecuritySettings` | Gets the security settings applied to the out-of-browser application. <br>Returns: The security settings applied to the out-of-browser application. |
| `ShortName` | Gets the short version of the application title. <br>Returns: The short version of the application title. |
| `ShowInstallMenuItem` | Gets a value that indicates whether the application right-click menu includes an install option. <br>Returns: true if the right-click menu includes an install option; otherwise, false. The default is true. |
| `WindowSettings` | Gets the settings applied to the application window. <br>Returns: The settings applied to the application window. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BlurbProperty` | Identifies the `Blurb` dependency property. <br>Returns: The identifier for the `Blurb` dependency property. |
| `EnableGPUAccelerationProperty` | Identifies the `EnableGPUAcceleration` dependency property. <br>Returns: The identifier for the `EnableGPUAcceleration` dependency property. |
| `IconsProperty` | Identifies the `Icons` dependency property. <br>Returns: The identifier for the `Icons` dependency property. |
| `SecuritySettingsProperty` | Identifies the `SecuritySettings` dependency property. <br>Returns: The identifier for the `SecuritySettings` dependency property. |
| `ShortNameProperty` | Identifies the `ShortName` dependency property. <br>Returns: The identifier for the `ShortName` dependency property. |
| `ShowInstallMenuItemProperty` | Identifies the `ShowInstallMenuItem` dependency property. <br>Returns: The identifier for the `ShowInstallMenuItem` dependency property. |
| `WindowSettingsProperty` | Identifies the `WindowSettings` dependency property. <br>Returns: The identifier for the `WindowSettings` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `OutOfBrowserSettings` class. |

## System.Windows.Point

Represents an x- and y-coordinate pair in two-dimensional space. Can also represent a logical point for certain property usages.

### Properties / fields

| Member | Description |
|---|---|
| `X` | Gets or sets the `X`-coordinate value of this `Point` structure. <br>Returns: The `X`-coordinate value of this `Point` structure. The default value is 0. |
| `Y` | Gets or sets the `Y`-coordinate value of this `Point`. <br>Returns: The `Y`-coordinate value of this `Point` structure. The default value is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double)` | Initializes a `Point` structure that contains the specified values. <br>`x`: The x-coordinate value of the `Point` structure. <br>`y`: The y-coordinate value of the `Point` structure. |
| `Equals(Object)` | Determines whether the specified object is a `Point` and whether it contains the same values as this `Point`. <br>`o`: The object to compare. <br>Returns: true if `obj` is a `Point` and contains the same `X` and `Y` values as this `Point`; otherwise, false. |
| `Equals(Point)` | Compares two `Point` structures for equality. <br>`value`: The point to compare to this instance. <br>Returns: true if both `Point` structures contain the same `X` and `Y` values; otherwise, false. |
| `GetHashCode` | Returns the hash code for this `Point`. <br>Returns: The hash code for this `Point` structure. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `IFormatProvider)`. <br>`format`: The string specifying the format to use. -or- null to use the default format defined for the type of the IFormattable implementation. <br>`provider`: The IFormatProvider to use to format the value. -or- null to obtain the numeric format information from the current locale setting of the operating system. <br>Returns: A string containing the value of the current instance in the specified format. |
| `ToString` | Creates a `String` representation of this `Point`. <br>Returns: A `String` containing the `X` and `Y` values of this `Point` structure. |
| `ToString(IFormatProvider)` | Creates a `String` representation of this `Point`. <br>`provider`: Culture-specific formatting information. <br>Returns: A `String` containing the `X` and `Y` values of this `Point` structure. |
| `op_Equality(Point,Point)` | Compares two `Point` structures for equality. <br>`point1`: The first `Point` structure to compare. <br>`point2`: The second `Point` structure to compare. <br>Returns: true if both the `X` and `Y` values of `point1` and `point2` are equal; otherwise, false. |
| `op_Inequality(Point,Point)` | Compares two `Point` structures for inequality <br>`point1`: The first point to compare. <br>`point2`: The second point to compare. <br>Returns: true if `point1` and `point2` have different `X` or `Y` values; false if `point1` and `point2` have the same `X` and `Y` values. |

## System.Windows.PresentationFrameworkCollection`1

Provides a common collection class for Silverlight collections.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of elements contained in the `PresentationFrameworkCollection`1`. <br>Returns: The number of elements contained in the `PresentationFrameworkCollection`1`. |
| `IsFixedSize` | Gets a value indicating whether the `PresentationFrameworkCollection`1` has a fixed size. <br>Returns: true if the `PresentationFrameworkCollection`1` has a fixed size; otherwise, false. |
| `IsReadOnly` | Gets a value indicating whether the `PresentationFrameworkCollection`1` is read-only. <br>Returns: true if the `PresentationFrameworkCollection`1` is read-only; otherwise, false. |
| `IsSynchronized` | Gets a value indicating whether access to the `PresentationFrameworkCollection`1` is synchronized (thread safe). <br>Returns: true if access to the `PresentationFrameworkCollection`1` is synchronized (thread safe); otherwise, false. |
| `Item` | Gets or sets the element at the specified index. <br>`index`: The zero-based index of the element to get or set. <br>Returns: The element at the specified index. |
| `SyncRoot` | Gets an object that can be used to synchronize access to the `PresentationFrameworkCollection`1` . <br>Returns: An object that can be used to synchronize access to the `PresentationFrameworkCollection`1`. |
| `System#Collections#IList#Item` | Gets or sets the element at the specified index. <br>`index`: The zero-based index of the element to get or set. <br>Returns: The element at the specified index. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CountProperty` | Identifies the `Count` dependency property. <br>Returns: The identifier for the `Count` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Add(`0)` | Adds an item to the `PresentationFrameworkCollection`1`. <br>`value`: The object to add. |
| `Clear` | Removes all items from the `PresentationFrameworkCollection`1`. |
| `Contains(`0)` | Determines whether the `PresentationFrameworkCollection`1` contains a specific value. <br>`value`: The object to locate in the `PresentationFrameworkCollection`1`. <br>Returns: true if the object is found in the `PresentationFrameworkCollection`1`; otherwise, false. |
| `CopyTo(Array,Int32)` | Copies the elements of the `PresentationFrameworkCollection`1` to an `Array`, starting at a particular `Array` index. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from the `PresentationFrameworkCollection`1`. The `Array` must have zero-based indexing. <br>`index`: The zero-based index in at which copying begins. |
| `CopyTo(`0[],Int32)` | Copies the elements of the `PresentationFrameworkCollection`1` to an `Array`, starting at a particular `Array` index. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from the `PresentationFrameworkCollection`1`. The `Array` must have zero-based indexing. <br>`index`: The zero-based index in at which copying begins. |
| `GetEnumerator` | Returns an enumerator that iterates through a collection. <br>Returns: An `IEnumerator` object that can be used to iterate through the collection. |
| `IndexOf(`0)` | Determines the index of a specific item in the `PresentationFrameworkCollection`1`. <br>`value`: The object to locate in the `PresentationFrameworkCollection`1`. <br>Returns: The index of if found in the list; otherwise, -1. |
| `Insert(Int32,`0)` | Inserts an item to the `PresentationFrameworkCollection`1` at the specified index. <br>`index`: The zero-based index at which should be inserted. <br>`value`: The object to insert into the `PresentationFrameworkCollection`1`. |
| `Remove(`0)` | Removes the first occurrence of a specific object from the `PresentationFrameworkCollection`1`. <br>`value`: The object to remove from the `PresentationFrameworkCollection`1`. <br>Returns: true if an object was removed; otherwise, false. |
| `RemoveAt(Int32)` | Removes the item at the specified index. <br>`index`: The zero-based index of the item to remove. |
| `System#Collections#IEnumerable#GetEnumerator` | Returns an enumerator that iterates through a collection. <br>Returns: An `IEnumerator` object that can be used to iterate through the collection. |
| `System#Collections#IList#Add(Object)` | Adds an item to the `PresentationFrameworkCollection`1`. <br>`value`: The object to add. <br>Returns: The position into which the new element was inserted. |
| `System#Collections#IList#Contains(Object)` | Determines whether the `PresentationFrameworkCollection`1` contains a specific value. <br>`value`: The object to locate in the `PresentationFrameworkCollection`1`. <br>Returns: true if the object is found in the `PresentationFrameworkCollection`1`; otherwise, false. |
| `System#Collections#IList#IndexOf(Object)` | Determines the index of a specific item in the `PresentationFrameworkCollection`1`. <br>`value`: The object to locate in the `PresentationFrameworkCollection`1`. <br>Returns: The index of if found in the list; otherwise, -1. |
| `System#Collections#IList#Insert(Int32,Object)` | Inserts an item to the `PresentationFrameworkCollection`1` at the specified index. <br>`index`: The zero-based index at which should be inserted. <br>`value`: The object to insert into the `PresentationFrameworkCollection`1`. |
| `System#Collections#IList#Remove(Object)` | Removes the first occurrence of a specific object from the `PresentationFrameworkCollection`1`. <br>`value`: The object to remove from the `PresentationFrameworkCollection`1`. |

## System.Windows.Printing.BeginPrintEventArgs

Provides data for the `BeginPrint` event.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BeginPrintEventArgs` class. |

## System.Windows.Printing.EndPrintEventArgs

Provides data for the `EndPrint` event.

### Properties / fields

| Member | Description |
|---|---|
| `Error` | Gets an exception that indicates what kind of error occurred during printing, if an error occurred. <br>Returns: An exception that indicates what kind of error occurred during printing, if an error occurred. The default is null. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EndPrintEventArgs` class. |

## System.Windows.Printing.PrintDocument

Provides printing capabilities for a Silverlight application.

### Properties / fields

| Member | Description |
|---|---|
| `PrintedPageCount` | Gets the number of pages that have printed. <br>Returns: The number of pages that have printed. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `PrintedPageCountProperty` | Gets the identifier for the `PrintedPageCount` dependency property. <br>Returns: The identifier for the `PrintedPageCount` dependency property. |

### Events

| Member | Description |
|---|---|
| `BeginPrint` | Occurs after the `String)` method is called and the print dialog box successfully returns, but before the `PrintPage` event is raised. |
| `EndPrint` | Occurs when the printing operation is passed to the print spooler or when the print operation is cancelled by the application author. |
| `PrintPage` | Occurs when each page is printing. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PrintDocument` class. |
| `Print(String)` | Starts the printing process for the specified document by opening the print dialog box. <br>`documentName`: The name of the document to print. |

## System.Windows.Printing.PrintPageEventArgs

Provides data for the `PrintPage` event.

### Properties / fields

| Member | Description |
|---|---|
| `HasMorePages` | Gets or sets whether there are more pages to print. <br>Returns: true if there are additional pages to print; otherwise, false. The default is false. |
| `PageMargins` | Gets the margins of the page that is currently printing. <br>Returns: The margins of the page that is currently printing. |
| `PageVisual` | Gets or sets the visual element to print. <br>Returns: The visual element to print. The default is null. |
| `PrintableArea` | Gets the size of the printable area. <br>Returns: The size of the printable area in screen pixels. The default is 0.0F. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PrintPageEventArgs` class. |

## System.Windows.PropertyChangedCallback

Represents the callback that is invoked when the effective property value of a dependency property changes.

## System.Windows.PropertyMetadata

Defines certain behavior aspects of a dependency property, including conditions it was registered with.

### Properties / fields

| Member | Description |
|---|---|
| `DefaultValue` | Gets the default value for the dependency property. <br>Returns: The default value for the dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object)` | Initializes a new instance of the `PropertyMetadata` class, using a property default value. <br>`defaultValue`: A default value for the property where this `PropertyMetadata` is applied. |
| `Constructor(Object,PropertyChangedCallback)` | Initializes a new instance of the `PropertyMetadata` class, using a property default value and callback reference. <br>`defaultValue`: A default value for the property where this `PropertyMetadata` is applied. <br>`propertyChangedCallback`: A reference to the callback to call for property changed behavior. |
| `Constructor(PropertyChangedCallback)` | Initializes a new instance of the `PropertyMetadata` class, using a callback reference. <br>`propertyChangedCallback`: A reference to the callback to call for property changed behavior. |

## System.Windows.PropertyPath

Implements a data structure for describing a property as a path below another property, or below an owning type. Property paths are used in data binding to objects, and in storyboards and timelines for animations.

### Properties / fields

| Member | Description |
|---|---|
| `Path` | Gets the path value held by this `PropertyPath`. <br>Returns: The path value held by this `PropertyPath`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Object)` | Initializes a new instance of the `PropertyPath` class. <br>`parameter`: A dependency property identifier, or a property path string. |
| `Constructor(String,Object[])` | Initializes a new instance of the `PropertyPath` class. <br>`path`: The path string for this `PropertyPath`. <br>`pathParameters`: Do not use. See Remarks. |

## System.Windows.PropertyPathConverter

Provides type conversion support for the `PropertyPath` type.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PropertyPathConverter` class. |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Returns whether this converter can convert an object of one type to the `PropertyPath` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: A `Type` that represents the type you want to convert from. <br>Returns: true if this converter can perform the conversion; otherwise, false. |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the given value to the `PropertyPath` type. <br>`context`: An `ITypeDescriptorContext` that provides a format context. <br>`culture`: The `CultureInfo` to use as the current culture. <br>`value`: The object to convert. <br>Returns: The returned `PropertyPath`. |

## System.Windows.Rect

Describes the width, height, and point origin of a rectangle.

### Properties / fields

| Member | Description |
|---|---|
| `Bottom` | Gets the y-axis value of the bottom of the rectangle. <br>Returns: The y-axis value of the bottom of the rectangle. If the rectangle is empty, the value is `NegativeInfinity` . |
| `Empty` | Gets a special value that represents a rectangle with no position or area. <br>Returns: The empty rectangle, which has `X` and `Y` property values of `PositiveInfinity`, and has `Width` and `Height` property values of `NegativeInfinity`. |
| `Height` | Gets or sets the height of the rectangle. <br>Returns: A value that represents the height of the rectangle. The default is 0. |
| `IsEmpty` | Gets a value that indicates whether the rectangle is the `Empty` rectangle. <br>Returns: true if the rectangle is the `Empty` rectangle; otherwise, false. |
| `Left` | Gets the x-axis value of the left side of the rectangle. <br>Returns: The x-axis value of the left side of the rectangle. |
| `Right` | Gets the x-axis value of the right side of the rectangle. <br>Returns: The x-axis value of the right side of the rectangle. |
| `Top` | Gets the y-axis position of the top of the rectangle. <br>Returns: The y-axis position of the top of the rectangle. |
| `Width` | Gets or sets the width of the rectangle. <br>Returns: A value that represents the width of the rectangle in pixels. The default is 0. |
| `X` | Gets or sets the x-axis value of the left side of the rectangle. <br>Returns: The x-axis value of the left side of the rectangle. This value is interpreted as pixels within the coordinate space. |
| `Y` | Gets or sets the y-axis value of the top side of the rectangle. <br>Returns: The y-axis value of the top side of the rectangle. This value is interpreted as pixels within the coordinate space.. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double,Double,Double)` | Initializes a `Rect` structure that has the specified x-coordinate, y-coordinate, width, and height. <br>`x`: The x-coordinate of the top-left corner of the rectangle. <br>`y`: The y-coordinate of the top-left corner of the rectangle. <br>`width`: The width of the rectangle. <br>`height`: The height of the rectangle. |
| `Constructor(Point,Point)` | Initializes a `Rect` structure that is exactly large enough to contain the two specified points. <br>`point1`: The first point that the new rectangle must contain. <br>`point2`: The second point that the new rectangle must contain. |
| `Constructor(Point,Size)` | Initializes a `Rect` structure based on an origin and size. <br>`location`: The origin of the new `Rect`. <br>`size`: The size of the new `Rect`. |
| `Contains(Point)` | Indicates whether the rectangle described by the `Rect` contains the specified point. <br>`point`: The point to check. <br>Returns: true if the rectangle described by the `Rect` contains the specified point; otherwise, false. |
| `Equals(Object)` | Indicates whether the specified object is equal to the current `Rect`. <br>`o`: The object to compare to the current rectangle. <br>Returns: true if `o` is a `Rect` and has the same x,y,width,height values as the current `Rect`; otherwise, false. |
| `Equals(Rect)` | Indicates whether the specified `Rect` is equal to the current `Rect`. <br>`value`: The rectangle to compare to the current rectangle. <br>Returns: true if the specified `Rect` has the same x,y,width,height property values as the current `Rect`; otherwise, false. |
| `GetHashCode` | Creates a hash code for the `Rect`. <br>Returns: A hash code for the current `Rect` structure. |
| `Intersect(Rect)` | Finds the intersection of the rectangle represented by the current `Rect` and the rectangle represented by the specified `Rect`, and stores the result as the current `Rect`. <br>`rect`: The rectangle to intersect with the current rectangle. |
| `System#IFormattable#ToString(String,IFormatProvider)` | For a description of this member, see `IFormatProvider)`. <br>`format`: The string specifying the format to use. -or- null to use the default format defined for the type of the IFormattable implementation. <br>`provider`: The IFormatProvider to use to format the value. -or- null to obtain the numeric format information from the current locale setting of the operating system. <br>Returns: A string containing the value of the current instance in the specified format. |
| `ToString` | Returns a string representation of the `Rect` structure. <br>Returns: A string representation of the current `Rect` structure. The string has the following form: "`X`,`Y`,`Width`,`Height`". |
| `ToString(IFormatProvider)` | Returns a string representation of the rectangle by using the specified format provider. <br>`provider`: Culture-specific formatting information. <br>Returns: A string representation of the current rectangle that is determined by the specified format provider. |
| `Union(Point)` | Expands the rectangle represented by the current `Rect` exactly enough to contain the specified point. <br>`point`: The point to include. |
| `Union(Rect)` | Expands the rectangle represented by the current `Rect` exactly enough to contain the specified rectangle. <br>`rect`: The rectangle to include. |
| `op_Equality(Rect,Rect)` | Compares two `Rect` structures for equality. <br>`rect1`: The first rectangle to compare. <br>`rect2`: The second rectangle to compare. <br>Returns: true if the `Rect` structures have the same x,y,width,height property values; otherwise, false. |
| `op_Inequality(Rect,Rect)` | Compares two `Rect` structures for inequality. <br>`rect1`: The first rectangle to compare. <br>`rect2`: The second rectangle to compare. <br>Returns: true if the `Rect` structures do not have the same x,y,width,height property values; otherwise, false. |

## System.Windows.ResourceDictionary

Provides a dictionary that contains resources used by components of a Silverlight-based application.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of elements contained in the collection. <br>Returns: The number of elements contained in the collection. |
| `IsFixedSize` | Gets a value indicating whether the `ResourceDictionary` has a fixed size. <br>Returns: Always returns false. |
| `IsReadOnly` | Gets a value indicating whether the collection is read-only. <br>Returns: Always returns false. |
| `Item` | Gets the element at the specified index. <br>`key`: The key of the element to get. <br>Returns: The object if it exists; otherwise, null. |
| `Zeys` | Gets an `ICollection` object containing the keys of the `ResourceDictionary`. <br>Returns: The `ICollection` of all keys in this `ResourceDictionary`. |
| `MergedDictionaries` | Gets a collection of the `ResourceDictionary` dictionaries that constitute the various resource dictionaries in the merged dictionaries. <br>Returns: A collection of the `ResourceDictionary` dictionaries that constitute the various resource dictionaries in the merged dictionaries. |
| `Source` | Gets or sets a URI that provides the source location of a merged resource dictionary. <br>Returns: A URI that provides the source location of a merged resource dictionary. This is typically a path that references a XAML resource within the compiled DLL, or a relative reference to a loose XAML file in the XAP. |
| `System#Collections#Generic#ICollection{T}#Count` | For a description of this member, see `Count`. <br>Returns: The number of elements contained in the collection. |
| `System#Collections#Generic#ICollection{T}#IsReadOnly` | For a description of this member, see `IsReadOnly`. <br>Returns: true if the collection is read-only; otherwise, false. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Item` | For a description of this member, see `Item(`0)`. <br>`key`: The key of the element to get or set. <br>Returns: The element with the specified key. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Zeys` | For a description of this member, see `Zeys`. <br>Returns: A collection that contains the keys of the elements in the dictionary. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Values` | For a description of this member, see `Values`. <br>Returns: A collection that contains the values of the elements in the dictionary. |
| `System#Collections#ICollection#IsSynchronized` | For a description of this member, see `IsSynchronized`. <br>Returns: Always returns false. |
| `System#Collections#ICollection#SyncRoot` | For a description of this member, see `SyncRoot`. <br>Returns: Always null. `ResourceDictionary` is not synchronized. |
| `Values` | Gets an `ICollection` object containing the values of the `ResourceDictionary`. <br>Returns: The `ICollection` of all values in this `ResourceDictionary`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ResourceDictionary` class. |
| `Add(Object,Object)` | Adds an item to the `ResourceDictionary`. <br>`key`: The string key of the item to add. <br>`value`: The item value to add. |
| `Add(String,Object)` | Adds an item to the `ResourceDictionary`. <br>`key`: The string key of the item to add. <br>`value`: The item value to add. |
| `Clear` | Removes all items from this `ResourceDictionary`. |
| `Contains(Object)` | Returns a value that indicates whether a specified key exists in the `ResourceDictionary`. <br>`key`: The key to check for in the `ResourceDictionary`. <br>Returns: true if an item with that key exists in the `ResourceDictionary`; otherwise, false. |
| `CopyTo(Array,Int32)` | Copies the elements of the `ResourceDictionary` to an `Array`, starting at a particular `Array` index. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from `ResourceDictionary`. The `Array` must have zero-based indexing. <br>`index`: The zero-based index in at which copying begins. |
| `GetEnumerator` | Exposes the enumerator, which supports a simple iteration over a non-generic collection. <br>Returns: An enumerator that can be used to iterate through the collection. |
| `Remove(Object)` | Removes a specific item from the `ResourceDictionary`. <br>`key`: The string key of the item to remove. |
| `Remove(String)` | Removes a specific item from the `ResourceDictionary`. <br>`key`: The string key of the item to remove. |
| `System#Collections#Generic#ICollection{T}#Add(ZeyValuePair{Object,Object})` | For a description of this member, see `Add(`0)`. <br>`item`: The object to add to the collection. |
| `System#Collections#Generic#ICollection{T}#Clear` | For a description of this member, see `Clear`. |
| `System#Collections#Generic#ICollection{T}#Contains(ZeyValuePair{Object,Object})` | For a description of this member, see `Contains(`0)`. <br>`item`: The object to locate in the collection. <br>Returns: true if is found in the collection; otherwise, false. |
| `System#Collections#Generic#ICollection{T}#CopyTo(ZeyValuePair{Object,Object}[],Int32)` | For a description of this member, see `Int32)`. <br>`array`: The one-dimensional array that is the destination of the elements copied from the collection. The array must have zero-based indexing. <br>`arrayIndex`: The zero-based index in at which copying begins. |
| `System#Collections#Generic#ICollection{T}#Remove(ZeyValuePair{Object,Object})` | For a description of this member, see `Remove(`0)`. <br>`item`: The object to remove from the collection. <br>Returns: true if was successfully removed from the collection; otherwise, false. This method also returns false if is not found in the original collection. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Add(Object,Object)` | For a description of this member, see `Add(`0,`1)`. <br>`key`: The object to use as the key for the element to add. <br>`value`: The object to use as the value for the element to add. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#ContainsKey(Object)` | For a description of this member, see `ContainsKey(`0)`. <br>`key`: The key to locate in the dictionary. <br>Returns: true if the dictionary contains an element with the key; otherwise, false. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#Remove(Object)` | For a description of this member, see `Remove(`0)`. <br>`key`: The key of the element to remove. <br>Returns: true if the element was successfully removed from the collection; otherwise, false. This method also returns false if is not found in the original collection. |
| `System#Collections#Generic#IDictionary{TKey@TValue}#TryGetValue(Object,Object@)` | For a description of this member, see `TryGetValue(`0,`1@)`. <br>`key`: The key whose value to get. <br>`value`: When this method returns, the value associated with the specified key, if the key is found; otherwise, the default value for the type of the value parameter. This parameter is passed uninitialized. <br>Returns: true if the dictionary contains an object with the specified key; otherwise, false. |
| `System#Collections#Generic#IEnumerable{T}#GetEnumerator` | For a description of this member, see `GetEnumerator`. <br>Returns: An enumerator that can be used to iterate through the collection. |
| `System#Collections#IEnumerable#GetEnumerator` | For a description of this member, see `GetEnumerator`. <br>Returns: An enumerator that can be used to iterate through the collection. |

## System.Windows.Resources.StreamResourceInfo

Provides resource stream information for application resources or other packages obtained through the `Uri)` method.

### Properties / fields

| Member | Description |
|---|---|
| `ContentType` | Gets the MIME type of the content in the stream. <br>Returns: The MIME type of the content in the stream, as a string. |
| `Stream` | Gets the stream that is contained by the resource. <br>Returns: The stream that is contained by the resource. |

### Methods

| Member | Description |
|---|---|
| `Constructor(IO.Stream,String)` | Initializes a new instance of the `StreamResourceInfo` class. <br>`stream`: The stream to use to create the resource. <br>`contentType`: The MIME type of the content. |

## System.Windows.RoutedEvent

Represents a routed event to the Silverlight event system.

### Methods

| Member | Description |
|---|---|
| `ToString` | Returns the string representation of the routed event. <br>Returns: The name of the routed event. |

## System.Windows.RoutedEventArgs

Contains state information and event data associated with a routed event.

### Properties / fields

| Member | Description |
|---|---|
| `OriginalSource` | Gets a reference to the object that raised the event. <br>Returns: The object that raised the event. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `RoutedEventArgs` class. |

## System.Windows.RoutedEventHandler

Represents the method that will handle routed events.

## System.Windows.RoutedPropertyChangedEventArgs`1

Provides data about a change in value to a dependency property as reported by particular routed events, including the previous and current value of the property that changed.

### Properties / fields

| Member | Description |
|---|---|
| `NewValue` | Gets the new value of a property as reported by a property-changed event. <br>Returns: The generic value. In a practical implementation of the `RoutedPropertyChangedEventArgs`1`, the generic type of this property is replaced with the constrained type of the implementation. |
| `OldValue` | Gets the previous value of the property as reported by a property-changed event. <br>Returns: The generic value. In a practical implementation of the `RoutedPropertyChangedEventArgs`1`, the generic type of this property is replaced with the constrained type of the implementation. |

### Methods

| Member | Description |
|---|---|
| `Constructor(`0,`0)` | Initializes a new instance of the `RoutedPropertyChangedEventArgs`1` class, with provided old and new values. <br>`oldValue`: The previous value of the property, before the event was raised. <br>`newValue`: The current value of the property at the time of the event. |

## System.Windows.RoutedPropertyChangedEventHandler`1

Represents methods that will handle various routed events that track property value changes.

## System.Windows.SecuritySettings

Represents the security configuration of an out-of-browser application.

### Properties / fields

| Member | Description |
|---|---|
| `ElevatedPermissions` | Gets a value that indicates whether the out-of-browser application requires elevated permissions. <br>Returns: A value that indicates whether the out-of-browser application requires elevated permissions. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ElevatedPermissionsProperty` | Identifies the `ElevatedPermissions` dependency property. <br>Returns: The identifier for the `ElevatedPermissions` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SecuritySettings` class. |

## System.Windows.Setter

Applies a value to a property in a `Style`.

### Properties / fields

| Member | Description |
|---|---|
| `Property` | Gets or sets the property to apply the `Value` to. <br>Returns: A `DependencyProperty` to which the `Value` will be applied. The default is null. |
| `Value` | Gets or sets the value to apply to the property that is specified by the `Setter`. <br>Returns: The value to apply to the property that is specified by the `Setter`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Setter` class. |
| `Constructor(DependencyProperty,Object)` | Initializes a new instance of the `Setter` class with the specified property and value. <br>`property`: The dependency property to apply the value to. <br>`value`: The value to apply to the property. |

## System.Windows.SetterBase

Represents the base class for value setters.

### Properties / fields

| Member | Description |
|---|---|
| `IsSealed` | Gets a value that indicates whether this object is in an immutable state. <br>Returns: true if this object is in an immutable state; otherwise, false. |

## System.Windows.SetterBaseCollection

Represents a collection of objects that inherit from `SetterBase`.

### Properties / fields

| Member | Description |
|---|---|
| `IsSealed` | Gets a value that indicates whether the collection is in a read-only state. <br>Returns: true if this object is in a read-only state and cannot be changed; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SetterBaseCollection` class. |

## System.Windows.Shapes.Ellipse

Draws an ellipse.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Ellipse` class. |

## System.Windows.Shapes.Line

Draws a straight line between two points.

### Properties / fields

| Member | Description |
|---|---|
| `X1` | Gets or sets the x-coordinate of the `Line` start point. <br>Returns: The x-coordinate for the start point of the line, in pixels. The default is 0. |
| `X2` | Gets or sets the x-coordinate of the `Line` end point. <br>Returns: The x-coordinate for the end point of the line, in pixels. The default is 0. |
| `Y1` | Gets or sets the y-coordinate of the `Line` start point. <br>Returns: The y-coordinate for the start point of the line, in pixels. The default is 0. |
| `Y2` | Gets or sets the y-coordinate of the `Line` end point. <br>Returns: The y-coordinate for the end point of the line, in pixels. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `X1Property` | Identifies the `X1` dependency property. <br>Returns: The identifier for the `X1` dependency property. |
| `X2Property` | Identifies the `X2` dependency property. <br>Returns: The identifier for the `X2` dependency property. |
| `Y1Property` | Identifies the `Y1` dependency property. <br>Returns: The identifier for the `Y1` dependency property. |
| `Y2Property` | Identifies the `Y2` dependency property. <br>Returns: The identifier for the `Y2` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Line` class. |

## System.Windows.Shapes.Path

Draws a series of connected lines and curves. The line and curve dimensions are declared through the `Data` property, and can be specified either with a Path-specific mini-language, or with an object model.

### Properties / fields

| Member | Description |
|---|---|
| `Data` | Gets or sets a `Geometry` that specifies the shape to be drawn. <br>Returns: A description of the shape to be drawn. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DataProperty` | Identifies the `Data` dependency property. <br>Returns: The identifier for the `Data` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Path` class. |

## System.Windows.Shapes.Polygon

Draws a polygon, which is a connected series of lines that form a closed shape.

### Properties / fields

| Member | Description |
|---|---|
| `FillRule` | Gets or sets a value that specifies how the interior fill of the shape is determined. <br>Returns: A value of the enumeration. The default is `EvenOdd`. |
| `Points` | Gets or sets a collection that contains the vertex points of the polygon. <br>Returns: A collection of `Point` structures that describes the vertex points of the polygon. The default is null. The value can be expressed as a string as described in "pointSet Grammar" below. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FillRuleProperty` | Identifies the `FillRule` dependency property. <br>Returns: The identifier for the `FillRule` dependency property. |
| `PointsProperty` | Identifies the `Points` dependency property. <br>Returns: The identifier for the `Points` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Polygon` class. |

## System.Windows.Shapes.Polyline

Draws a series of connected straight lines.

### Properties / fields

| Member | Description |
|---|---|
| `FillRule` | Gets or sets a value that specifies how the interior fill of the shape is determined. <br>Returns: A value of the enumeration that specifies the fill behavior. The default is `EvenOdd`. |
| `Points` | Gets or sets a collection that contains the vertex points of the `Polyline`. <br>Returns: A collection of `Point` structures that describe the vertex points of the `Polyline`. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FillRuleProperty` | Identifies the `FillRule` dependency property. <br>Returns: The identifier for the `FillRule` dependency property. |
| `PointsProperty` | Identifies the `Points` dependency property. <br>Returns: The identifier for the `Points` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Polyline` class. |

## System.Windows.Shapes.Rectangle

Draws a rectangle shape, which can have a stroke and a fill.

### Properties / fields

| Member | Description |
|---|---|
| `RadiusX` | Gets or sets the x-axis radius of the ellipse that is used to round the corners of the rectangle. <br>Returns: The x-axis radius of the ellipse that is used to round the corners of the rectangle. |
| `RadiusY` | Gets or sets the y-axis radius of the ellipse that is used to round the corners of the rectangle. <br>Returns: The y-axis radius of the ellipse that is used to round the corners of the rectangle. The default is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `RadiusXProperty` | Identifies the `RadiusX` dependency property. <br>Returns: The identifier for the `RadiusX` dependency property. |
| `RadiusYProperty` | Identifies the `RadiusY` dependency property. <br>Returns: The identifier for the `RadiusY` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Rectangle` class. |

## System.Windows.Shapes.Shape

Provides a base class for shape elements, such as `Ellipse`, `Polygon`, and `Rectangle`.

### Properties / fields

| Member | Description |
|---|---|
| `Fill` | Gets or sets the `Brush` that specifies how to paint the interior of the shape. <br>Returns: A `Brush` that describes how the shape's interior is painted. The default is null. See Remarks. |
| `GeometryTransform` | Gets a value that represents a `Transform` that is applied to the geometry of a `Shape` prior to when it is drawn. <br>Returns: A `Transform` that is applied to the geometry of a `Shape` prior to when it is drawn. |
| `Stretch` | Gets or sets a `Stretch` enumeration value that describes how the shape fills its allocated space. <br>Returns: One of the `Stretch` enumeration values. The default value at run time depends on the type of `Shape`. |
| `Stroke` | Gets or sets the `Brush` that specifies how the `Shape` outline is painted. <br>Returns: A `Brush` that specifies how the `Shape` outline is painted. The default is null. |
| `StrokeDashArray` | Gets or sets a collection of `Double` values that indicate the pattern of dashes and gaps that is used to outline shapes. <br>Returns: A collection of `Double` values that specify the pattern of dashes and gaps. |
| `StrokeDashCap` | Gets or sets a `PenLineCap` enumeration value that specifies how the ends of a dash are drawn. <br>Returns: One of the enumeration values for `PenLineCap`. The default is `Flat`. |
| `StrokeDashOffset` | Gets or sets a `Double` that specifies the distance within the dash pattern where a dash begins. <br>Returns: A `Double` that represents the distance within the dash pattern where a dash begins. The default value is 0. |
| `StrokeEndLineCap` | Gets or sets a `PenLineCap` enumeration value that describes the `Shape` at the end of a line. <br>Returns: One of the enumeration values for `PenLineCap`. The default is `Flat`. |
| `StrokeLineJoin` | Gets or sets a `PenLineJoin` enumeration value that specifies the type of join that is used at the vertices of a `Shape`. <br>Returns: A value of the `PenLineJoin` enumeration that specifies the join appearance. The default value is `Miter`. |
| `StrokeMiterLimit` | Gets or sets a limit on the ratio of the miter length to half the `StrokeThickness` of a `Shape` element. <br>Returns: The limit on the ratio of the miter length to the `StrokeThickness` of a `Shape` element. This value is always a positive number that is greater than or equal to 1. |
| `StrokeStartLineCap` | Gets or sets a `PenLineCap` enumeration value that describes the `Shape` at the start of a `Stroke`. <br>Returns: A value of the `PenLineCap` enumeration that specifies the shape at the start of a `Stroke`. The default is `Flat`. |
| `StrokeThickness` | Gets or sets the width of the `Shape` stroke outline. <br>Returns: The width of the `Shape` outline, in pixels. The default value is 0. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FillProperty` | Identifies the `Fill` dependency property. <br>Returns: The identifier for the `Fill` dependency property. |
| `StretchProperty` | Identifies the `Stretch` dependency property. <br>Returns: The identifier for the `Stretch` dependency property. |
| `StrokeDashArrayProperty` | Identifies the `StrokeDashArray` dependency property. <br>Returns: The identifier for the `StrokeDashArray` dependency property. |
| `StrokeDashCapProperty` | Identifies the `StrokeDashCap` dependency property. <br>Returns: The identifier for the `StrokeDashCap` dependency property. |
| `StrokeDashOffsetProperty` | Identifies the `StrokeDashOffset` dependency property. <br>Returns: The identifier for the `StrokeDashOffset` dependency property. |
| `StrokeEndLineCapProperty` | Identifies the `StrokeEndLineCap` dependency property. <br>Returns: The identifier for the `StrokeEndLineCap` dependency property. |
| `StrokeLineJoinProperty` | Identifies the `StrokeLineJoin` dependency property. <br>Returns: The identifier for the `StrokeLineJoin` dependency property. |
| `StrokeMiterLimitProperty` | Identifies the `StrokeMiterLimit` dependency property. <br>Returns: The identifier for the `StrokeMiterLimit` dependency property. |
| `StrokeProperty` | Identifies the `Stroke` dependency property. <br>Returns: The identifier for the `Stroke` dependency property. |
| `StrokeStartLineCapProperty` | Identifies the `StrokeStartLineCap` dependency property. <br>Returns: The identifier for the `StrokeStartLineCap` dependency property. |
| `StrokeThicknessProperty` | Identifies the `StrokeThickness` dependency property. <br>Returns: The identifier for the `StrokeThickness` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Shape` class. |

## System.Windows.Size

Describes the width and height of an object.

### Properties / fields

| Member | Description |
|---|---|
| `Empty` | Gets a value that represents a static empty `Size`. <br>Returns: An empty instance of `Size`. |
| `Height` | Gets or sets the height of this instance of `Size`. <br>Returns: The `Height` of this instance of `Size`, in pixels. The default is 0. The value cannot be negative. |
| `IsEmpty` | Gets a value that indicates whether this instance of `Size` is `Empty`. <br>Returns: true if this instance of size is `Empty`; otherwise false. |
| `Width` | Gets or sets the width of this instance of `Size`. <br>Returns: The `Width` of this instance of `Size`, in pixels. The default value is 0. The value cannot be negative. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double,Double)` | Initializes a new instance of the `Size` structure and assigns it an initial `width` and `height`. <br>`width`: The initial width of the instance of `Size`. <br>`height`: The initial height of the instance of `Size`. |
| `Equals(Object)` | Compares an object to an instance of `Size` for equality. <br>`o`: The `Object` to compare. <br>Returns: true if the sizes are equal; otherwise, false. |
| `Equals(Size)` | Compares a value to an instance of `Size` for equality. <br>`value`: The size to compare to this current instance of `Size`. <br>Returns: true if the instances of `Size` are equal; otherwise, false. |
| `GetHashCode` | Gets the hash code for this instance of `Size`. <br>Returns: The hash code for this instance of `Size`. |
| `ToString` | Returns a string representation of this `Size`. <br>Returns: A string representation of this `Size`. |
| `op_Equality(Size,Size)` | Compares two instances of `Size` for equality. <br>`size1`: The first instance of `Size` to compare. <br>`size2`: The second instance of `Size` to compare. <br>Returns: true if the two instances of `Size` are equal; otherwise false. |
| `op_Inequality(Size,Size)` | Compares two instances of `Size` for inequality. <br>`size1`: The first instance of `Size` to compare. <br>`size2`: The second instance of `Size` to compare. <br>Returns: true if the instances of `Size` are not equal; otherwise false. |

## System.Windows.SizeChangedEventArgs

Provides data related to the `SizeChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `NewSize` | Gets the new size of the object reporting the size change. <br>Returns: The new size. The `Size` structure contains the new height and width. |
| `PreviousSize` | Gets the previous size of the object reporting the size change. <br>Returns: The previous size. |

## System.Windows.SizeChangedEventHandler

Represents the method that will handle the `SizeChanged` event.

## System.Windows.StartupEventArgs

Contains the event data for the `Startup` event.

### Properties / fields

| Member | Description |
|---|---|
| `InitParams` | Gets the initialization parameters that were passed as part of HTML initialization of a Silverlight plug-in. <br>Returns: The set of initialization parameters, as a dictionary with key strings and value strings. |

## System.Windows.StartupEventHandler

Represents the method that will handle the `Startup` event.

## System.Windows.Style

Contains property setters that can be shared between instances of a type.

### Properties / fields

| Member | Description |
|---|---|
| `BasedOn` | Gets or sets a defined style that is the basis of the current style. <br>Returns: A defined style that is the basis of the current style. The default value is null. |
| `IsSealed` | Gets a value that indicates whether the style is read-only and cannot be changed. <br>Returns: true if the style is read-only; otherwise, false. |
| `Setters` | Gets a collection of `Setter` objects. <br>Returns: A collection of `Setter` objects. The default is an empty collection. |
| `TargetType` | Gets or sets the type for which the style is intended. <br>Returns: The type of object to which the style is applied. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Style` class. |
| `Constructor(Type)` | Initializes a new instance of the `Style` class to use on the specified `Type`. <br>`targetType`: The type to which the style will apply. |
| `Seal` | Locks the style so that the `TargetType` property or any `Setter` in the `Setters` collection cannot be changed. |

## System.Windows.StyleTypedPropertyAttribute

Represents an attribute that is applied to the class definition and reports the `TargetType` of the properties that are of type `Style`.

### Properties / fields

| Member | Description |
|---|---|
| `Property` | Gets or sets the name of the property that is of type `Style`. <br>Returns: The name of the property. |
| `StyleTargetType` | Gets or sets the `TargetType` of the `Property` this attribute is specifying. <br>Returns: The `TargetType` of the `Property` this attribute is specifying. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `StyleTypedPropertyAttribute` class. |

## System.Windows.SystemColors

Contains system colors, system brushes, and system resource keys that correspond to system display elements.

### Properties / fields

| Member | Description |
|---|---|
| `ActiveBorderColor` | Gets a `Color` structure that is the color of the active window's border. <br>Returns: The color of the active window's border. |
| `ActiveCaptionColor` | Gets a `Color` structure that is the background color of the active window's title bar. <br>Returns: The background color of the active window's title bar. |
| `ActiveCaptionTextColor` | Gets a `Color` structure that is the color of the text in the active window's title bar. <br>Returns: The color of the active window's title bar. |
| `AppWorkspaceColor` | Gets a `Color` structure that is the color of the application workspace. <br>Returns: The color of the application workspace. |
| `ControlColor` | Gets a `Color` structure that is the face color of a three-dimensional display element. <br>Returns: The face color of a three-dimensional display element. |
| `ControlDarkColor` | Gets a `Color` structure that is the shadow color of a three-dimensional display element. <br>Returns: The shadow color of a three-dimensional display element. |
| `ControlDarkDarkColor` | Gets a `Color` structure that is the dark shadow color of a three-dimensional display element. <br>Returns: The dark shadow color of a three-dimensional display element. |
| `ControlLightColor` | Gets a `Color` structure that is the light color of a three-dimensional display element. <br>Returns: The light color of a three-dimensional display element. |
| `ControlLightLightColor` | Gets a `Color` structure that is the highlight color of a three-dimensional display element. <br>Returns: The highlight color of a three-dimensional display element. |
| `ControlTextColor` | Gets a `Color` structure that is the color of text in a three-dimensional display element. <br>Returns: The color of text in a three-dimensional display element. |
| `DesktopColor` | Gets a `Color` structure that is the color of the desktop. <br>Returns: The color of the desktop. |
| `GrayTextColor` | Gets a `Color` structure that is the color of disabled text. <br>Returns: The color of disabled text. |
| `HighlightColor` | Gets a `Color` structure that is the background color of selected items. <br>Returns: The background color of selected items. |
| `HighlightTextColor` | Gets a `Color` structure that is the color of the text of selected items. <br>Returns: The color of the text of selected items. |
| `InactiveBorderColor` | Gets a `Color` structure that is the color of an inactive window's border. <br>Returns: The color of an inactive window's border. |
| `InactiveCaptionColor` | Gets a `Color` structure that is the background color of an inactive window's title bar. <br>Returns: The background color of an inactive window's title bar. |
| `InactiveCaptionTextColor` | Gets a `Color` structure that is the color of the text of an inactive window's title bar. <br>Returns: The color of the text of an inactive window's title bar. |
| `InfoColor` | Gets a `Color` structure that is the background color for the `ToolTip` control. <br>Returns: The background color for the `ToolTip` control. |
| `InfoTextColor` | Gets a `Color` structure that is the text color for the `ToolTip` control. <br>Returns: The text color for the `ToolTip` control. |
| `MenuColor` | Gets a `Color` structure that is the color of a menu's background. <br>Returns: The color of a menu's background. |
| `MenuTextColor` | Gets a `Color` structure that is the color of a menu's text. <br>Returns: The color of a menu's text. |
| `ScrollBarColor` | Gets a `Color` structure that is the background color of a scroll bar. <br>Returns: The background color of a scroll bar. |
| `WindowColor` | Gets a `Color` structure that is the background color in the client area of a window. <br>Returns: The background color in the client area of a window. |
| `WindowFrameColor` | Gets a `Color` structure that is the color of a window frame. <br>Returns: The color of a window frame. |
| `WindowTextColor` | Gets a `Color` structure that is the color of the text in the client area of a window. <br>Returns: The color of the text in the client area of a window. |

## System.Windows.SystemParameters

Contains properties that you can use to query system settings.

### Properties / fields

| Member | Description |
|---|---|
| `HighContrast` | Gets a value that indicates whether the client computer is in high-contrast mode. <br>Returns: true if the client computer is in high-contrast mode; otherwise, false. |
| `WheelScrollLines` | Gets a value that indicates the number of lines to scroll vertically in response to mouse wheel events. <br>Returns: The number of lines to scroll vertically in response to mouse wheel events. |

## System.Windows.TemplateBindingExpression

Supports template binding.

## System.Windows.TemplatePartAttribute

Represents an attribute that is applied to the class definition to identify the types of the named parts that are used for control templating.

### Properties / fields

| Member | Description |
|---|---|
| `Name` | Gets or sets a string that reports the object model name of a template part. <br>Returns: A string that should be used as the name of a template part in a template. |
| `Type` | Gets or sets a string that reports the `Type` of a template part. <br>Returns: A `Type` object that reports the type of a template part. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TemplatePartAttribute` class |

## System.Windows.TemplateVisualStateAttribute

Specifies that a control can be in a certain state and that a `VisualState` is expected in the control's `ControlTemplate`.

### Properties / fields

| Member | Description |
|---|---|
| `GroupName` | Gets or sets the name of the group that the state belongs to. <br>Returns: The name of the group that the state belongs to. |
| `Name` | Gets or sets the name of the state that the control can be in. <br>Returns: The name of the state that the control can be in. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TemplateVisualStateAttribute` class. |

## System.Windows.TextAlignment

Specifies whether text is centered, left-aligned, or right-aligned.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Center` | Text is centered within the container. |
| `Justify` | Text is justified within the container. |
| `Left` | Text is aligned to the left edge of the container. |
| `Right` | Text is aligned to the right edge of the container. |

## System.Windows.TextDecorationCollection

Provides the value for the `TextDecorations` and `TextDecorations` properties.

## System.Windows.TextDecorations

Implements a set of predefined text decorations.

### Properties / fields

| Member | Description |
|---|---|
| `Underline` | Specifies an underlined text decoration. <br>Returns: A `TextDecorationCollection` that represents an underlined text decoration. |

## System.Windows.TextTrimming

Describes how text is trimmed when it overflows the edge of its containing box.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `None` | Text is not trimmed. |
| `WordEllipsis` | Text is trimmed at a word boundary. An ellipsis (...) is drawn in place of remaining text. |

## System.Windows.TextWrapping

Specifies whether text wraps when it reaches the edge of its container.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `NoWrap` | No line wrapping is performed. |
| `Wrap` | Line breaking occurs if a line of text overflows beyond the available width of its container. Line breaking occurs even if the standard line-breaking algorithm cannot determine any line break opportunity, such as when a line of text includes a long word that is constrained by a fixed-width container without scrolling. |

## System.Windows.Thickness

Describes the thickness of a frame around a rectangle. Four `Double` values describe the `Left`, `Top`, `Right`, and `Bottom` sides of the rectangle, respectively.

### Properties / fields

| Member | Description |
|---|---|
| `Bottom` | Gets or sets the width, in pixels, of the lower side of the bounding rectangle. <br>Returns: A `Double` that represents the width, in pixels, of the lower side of the bounding rectangle for this instance of `Thickness`. The default is 0. |
| `Left` | Gets or sets the width, in pixels, of the left side of the bounding rectangle. <br>Returns: A `Double` that represents the width, in pixels, of the left side of the bounding rectangle for this instance of `Thickness`. The default is 0. |
| `Right` | Gets or sets the width, in pixels, of the right side of the bounding rectangle. <br>Returns: A `Double` that represents the width, in pixels, of the right side of the bounding rectangle for this instance of `Thickness`. The default is 0. |
| `Top` | Gets or sets the width, in pixels, of the upper side of the bounding rectangle. <br>Returns: A `Double` that represents the width, in pixels, of the upper side of the bounding rectangle for this instance of `Thickness`. The default is 0. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Double)` | Initializes a `Thickness` structure that has the specified uniform length on each side. <br>`uniformLength`: The uniform length applied to all four sides of the bounding rectangle. |
| `Constructor(Double,Double,Double,Double)` | Initializes a `Thickness` structure that has specific lengths (supplied as a `Double`) applied to each side of the rectangle. <br>`left`: The thickness for the left side of the rectangle. <br>`top`: The thickness for the upper side of the rectangle. <br>`right`: The thickness for the right side of the rectangle <br>`bottom`: The thickness for the lower side of the rectangle. |
| `Equals(Object)` | Compares this `Thickness` structure to another `Object` for equality. <br>`obj`: The object to compare. <br>Returns: true if the two objects are equal; otherwise, false. |
| `Equals(Thickness)` | Compares this `Thickness` structure to another `Thickness` structure for equality. <br>`thickness`: An instance of `Thickness` to compare for equality. <br>Returns: true if the two instances of `Thickness` are equal; otherwise, false. |
| `GetHashCode` | Returns the hash code of the structure. <br>Returns: A hash code for this instance of `Thickness`. |
| `ToString` | Returns the string representation of the `Thickness` structure. <br>Returns: A `String` that represents the `Thickness` value. |
| `op_Equality(Thickness,Thickness)` | Compares the value of two `Thickness` structures for equality. <br>`t1`: The first structure to compare. <br>`t2`: The other structure to compare. <br>Returns: true if the two instances of `Thickness` are equal; otherwise, false. |
| `op_Inequality(Thickness,Thickness)` | Compares two `Thickness` structures for inequality. <br>`t1`: The first structure to compare. <br>`t2`: The other structure to compare. <br>Returns: true if the two instances of `Thickness` are not equal; otherwise, false. |

## System.Windows.Threading.Dispatcher

Provides services for managing the queue of work items for a thread.

### Methods

| Member | Description |
|---|---|
| `BeginInvoke(Action)` | Executes the specified delegate asynchronously on the thread the `Dispatcher` is associated with. <br>`a`: A delegate to a method that takes no arguments and does not return a value, which is pushed onto the `Dispatcher` event queue. <br>Returns: An object, which is returned immediately after `BeginInvoke` is called, that represents the operation that has been posted to the `Dispatcher` queue. |
| `BeginInvoke(Delegate,Object[])` | Executes the specified delegate asynchronously with the specified array of arguments on the thread the `Dispatcher` is associated with. <br>`d`: A delegate to a method that takes multiple arguments, which is pushed onto the `Dispatcher` event queue. <br>`args`: An array of objects to pass as arguments to the specified method. <br>Returns: An object, which is returned immediately after `BeginInvoke` is called, that represents the operation that has been posted to the `Dispatcher` queue. |
| `CheckAccess` | Determines whether the calling thread is the thread associated with this `Dispatcher`. <br>Returns: true if the calling thread is the thread associated with this `Dispatcher`; otherwise, false. |

## System.Windows.Threading.DispatcherOperation

Represents an operation that has been posted to the `Dispatcher` queue.

## System.Windows.Threading.DispatcherSynchronizationContext

Provides a synchronization context for Silverlight.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DispatcherSynchronizationContext` class by using the current `Dispatcher`. |
| `Constructor(Threading.Dispatcher)` | Initializes a new instance of the `DispatcherSynchronizationContext` class by using the specified `Dispatcher`. <br>`dispatcher`: The `Dispatcher` to associate this `DispatcherSynchronizationContext` with. |
| `CreateCopy` | Creates a copy of this `DispatcherSynchronizationContext`. <br>Returns: The copy of this synchronization context. |
| `Post(Threading.SendOrPostCallback,Object)` | Invokes the callback in the synchronization context asynchronously. <br>`d`: The delegate to call. <br>`state`: The object passed to the delegate. |
| `Send(Threading.SendOrPostCallback,Object)` | Invokes the callback in the synchronization context synchronously. <br>`d`: The delegate to call. <br>`state`: The object passed to the delegate. |

## System.Windows.Threading.DispatcherTimer

A timer that is integrated into the `Dispatcher` queue, which is processed at a specified interval of time and at a specified priority.

### Properties / fields

| Member | Description |
|---|---|
| `Interval` | Gets or sets the amount of time between timer ticks. <br>Returns: The amount of time between ticks. The default is 00:00:00. |
| `IsEnabled` | Gets a value that indicates whether the timer is running. <br>Returns: true if the timer is enabled; otherwise, false. |

### Events

| Member | Description |
|---|---|
| `Tick` | Occurs when the timer interval has elapsed. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DispatcherTimer` class. |
| `Start` | Starts the `DispatcherTimer`. |
| `Stop` | Stops the `DispatcherTimer`. |

## System.Windows.TriggerAction

Serves as the base class for `BeginStoryboard`.

## System.Windows.TriggerActionCollection

Represents a collection of `BeginStoryboard` objects.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TriggerActionCollection` class. |

## System.Windows.TriggerBase

Serves as the base class for `EventTrigger`.

## System.Windows.TriggerCollection

Represents a collection of `EventTrigger` objects.

## System.Windows.UIElement

`UIElement` is a base class for most of the objects that have visual appearance and can process basic input in Silverlight.

### Properties / fields

| Member | Description |
|---|---|
| `AllowDrop` | Gets or sets a value that determines whether this `UIElement` can be a drop target for purposes of Silverlight drag-and-drop operations. <br>Returns: true if this `UIElement` can be a drop target for purposes of Silverlight drag-and-drop operations; otherwise, false. The default is false. |
| `CacheMode` | Gets or sets a value that indicates that rendered content should be cached when possible. <br>Returns: A value that indicates that rendered content should be cached when possible. If you specify a value of `CacheMode`, rendering operations from `RenderTransform` and `Opacity` execute on the graphics processing unit (GPU), if available. The default is null, which does not enable a cached composition mode. |
| `Clip` | Gets or sets the `Geometry` used to define the outline of the contents of a `UIElement`. <br>Returns: The geometry to be used for clipping area sizing. The default value is null. |
| `DesiredSize` | Gets the size that this `UIElement` computed during the measure pass of the layout process. <br>Returns: The size that this `UIElement` computed during the measure pass of the layout process. |
| `Effect` | Gets or sets the pixel shader effect to use for rendering this `UIElement`. <br>Returns: The pixel shader effect to use for rendering this `UIElement`. The default is null (no effects). |
| `IsHitTestVisible` | Gets or sets whether the contained area of this `UIElement` can return true values for hit testing. <br>Returns: true if the contained area of this `UIElement` can be used for hit-testing; otherwise, false. The default is true. |
| `Opacity` | Gets or sets the degree of the object's opacity. <br>Returns: A value between 0 and 1.0 that declares the opacity factor, with 1.0 meaning full opacity and 0 meaning transparent. The default value is 1.0. |
| `OpacityMask` | Gets or sets the brush used to alter the opacity of regions of this object. <br>Returns: A brush that describes the opacity applied to this object. The default is null. |
| `Projection` | Gets or sets the perspective projection (3-D effect) to apply when rendering this `UIElement`. <br>Returns: The perspective projection to apply when rendering this `UIElement`. The default is null (no perspective applied). |
| `RenderSize` | Gets the final render size of a `UIElement`. <br>Returns: The rendered size for this object. There is no default value. |
| `RenderTransform` | Gets or sets transform information that affects the rendering position of a `UIElement`. <br>Returns: Describes the specifics of the desired render transform. The default value is null. |
| `RenderTransformOrigin` | Gets or sets the origin point of any possible render transform declared by `RenderTransform`, relative to the bounds of the `UIElement`. <br>Returns: The origin point of the render transform. The default value is a point with value 0,0. |
| `UseLayoutRounding` | Gets or sets a value that determines whether rendering for the object and its visual subtree should use rounding behavior that aligns rendering to whole pixels. <br>Returns: true if rendering and layout should use layout rounding to whole pixels; otherwise, false. The default is true. |
| `Visibility` | Gets or sets the visibility of a `UIElement`. A `UIElement` that is not visible does not render and does not communicate its desired size to layout. <br>Returns: A value of the enumeration. The default value is `Visible`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AllowDropProperty` | Identifies the `AllowDrop` dependency property. <br>Returns: The identifier for the `AllowDrop` dependency property. |
| `CacheModeProperty` | Identifies the `CacheMode` dependency property. <br>Returns: The identifier for the `CacheMode` dependency property. |
| `ClipProperty` | Identifies the `Clip` dependency property. <br>Returns: The identifier for the `Clip` dependency property. |
| `DoubleTapEvent` | Identifies the `DoubleTap` routed event. <br>Returns: The identifier for the `DoubleTap` routed event. |
| `EffectProperty` | Identifies the `Effect` dependency property. <br>Returns: The identifier for the `Effect` dependency property. |
| `HoldEvent` | Identifies the `Hold` routed event. <br>Returns: The identifier for the `Hold` routed event. |
| `IsHitTestVisibleProperty` | Identifies the `IsHitTestVisible` dependency property. <br>Returns: The identifier for the `IsHitTestVisible` dependency property. |
| `ZeyDownEvent` | Identifies the `ZeyDown` routed event. <br>Returns: The identifier for the `ZeyDown` routed event. |
| `ZeyUpEvent` | Identifies the `ZeyUp` routed event. <br>Returns: The identifier for the `ZeyUp` routed event. |
| `ManipulationCompletedEvent` | Identifies the `ManipulationCompleted` routed event. <br>Returns: The identifier for the `ManipulationCompleted` routed event. |
| `ManipulationDeltaEvent` | Identifies the `ManipulationDelta` routed event. <br>Returns: The identifier for the `ManipulationDelta` routed event. |
| `ManipulationStartedEvent` | Identifies the `ManipulationStarted` routed event. <br>Returns: The identifier for the `ManipulationStarted` routed event. |
| `MouseLeftButtonDownEvent` | Identifies the `MouseLeftButtonDown` routed event. <br>Returns: The identifier for the `MouseLeftButtonDown` routed event. |
| `MouseLeftButtonUpEvent` | Identifies the `MouseLeftButtonUp` routed event. <br>Returns: The identifier for the `MouseLeftButtonUp` routed event. |
| `MouseRightButtonDownEvent` | Identifies the `MouseRightButtonDown` routed event. <br>Returns: The identifier for the `MouseRightButtonDown` routed event. |
| `MouseRightButtonUpEvent` | Identifies the `MouseRightButtonUp` routed event. <br>Returns: The identifier for the `MouseRightButtonUp` routed event. |
| `MouseWheelEvent` | Identifies the `MouseWheel` routed event. <br>Returns: The identifier for the `MouseWheel` routed event. |
| `OpacityMaskProperty` | Identifies the `IsHitTestVisible` dependency property. <br>Returns: The identifier for the `Clip` dependency property. |
| `OpacityProperty` | Identifies the `IsHitTestVisible` dependency property. <br>Returns: The identifier for the `IsHitTestVisible` dependency property. |
| `ProjectionProperty` | Identifies the `Projection` dependency property. <br>Returns: The identifier for the `Projection` dependency property. |
| `RenderTransformOriginProperty` | Identifies the `RenderTransformOrigin` dependency property. <br>Returns: The identifier for the `RenderTransformOrigin` dependency property. |
| `RenderTransformProperty` | Identifies the `RenderTransform` dependency property. <br>Returns: The identifier for the `RenderTransform` dependency property. |
| `TapEvent` | Identifies the `Tap` routed event. <br>Returns: The identifier for the `Tap` routed event. |
| `TextInputEvent` | Identifies the `TextInput` routed event. <br>Returns: The identifier for the `TextInput` routed event. |
| `TextInputStartEvent` | Identifies the `TextInputStart` routed event. <br>Returns: The identifier for the `TextInputStart` routed event. |
| `TextInputUpdateEvent` | Identifies the `TextInputUpdate` routed event. <br>Returns: The identifier for the `TextInputUpdate` routed event. |
| `UseLayoutRoundingProperty` | Identifies the `UseLayoutRounding` dependency property. <br>Returns: The identifier for the `UseLayoutRounding` dependency property. |
| `VisibilityProperty` | Identifies the `Visibility` dependency property. <br>Returns: The identifier for the `Visibility` dependency property. |

### Events

| Member | Description |
|---|---|
| `DoubleTap` | Occurs when a DoubleTap gesture is committed while over this `UIElement`. |
| `DragEnter` | Occurs when the input system reports an underlying drag event with this element as the target. |
| `DragLeave` | Occurs when the input system reports an underlying drag event with this element as the origin. |
| `DragOver` | Occurs when the input system reports an underlying drag event with this element as the potential drop target. |
| `Drop` | Occurs when the input system reports an underlying drop event with this element as the drop target. |
| `GotFocus` | Occurs when a `UIElement` receives focus. |
| `Hold` | Occurs when a Hold gesture is committed while over this `UIElement`. |
| `ZeyDown` | Occurs when a keyboard key is pressed while the `UIElement` has focus. |
| `ZeyUp` | Occurs when a keyboard key is released while the `UIElement` has focus. |
| `LostFocus` | Occurs when a `UIElement` loses focus. |
| `LostMouseCapture` | Occurs when the `UIElement` loses mouse capture. |
| `ManipulationCompleted` | Occurs when a manipulation and inertia on the `UIElement` is complete. |
| `ManipulationDelta` | Occurs when the input device changes position during a manipulation. |
| `ManipulationStarted` | Occurs when an input device begins a manipulation on the `UIElement`. |
| `MouseEnter` | Occurs when the mouse (or a stylus) enters the bounding area of a `UIElement`. |
| `MouseLeave` | Occurs when the mouse (or the stylus) leaves the bounding area of a `UIElement`. |
| `MouseLeftButtonDown` | Occurs when the left mouse button is pressed (or when the tip of the stylus touches the tablet) while the mouse pointer is over a `UIElement`. |
| `MouseLeftButtonUp` | Occurs when the left mouse button is released (or the tip of the stylus is removed from the tablet) while the mouse (or the stylus) is over a `UIElement` (or while a `UIElement` holds mouse capture). |
| `MouseMove` | Occurs when the coordinate position of the mouse (or stylus) changes while over a `UIElement` (or while a `UIElement` holds mouse capture). |
| `MouseRightButtonDown` | Occurs when the right mouse button is pressed while the mouse pointer is over a `UIElement`. |
| `MouseRightButtonUp` | Occurs when the right mouse button is released while the mouse pointer is over a `UIElement`. However, this event will only be raised if a caller marks the preceding `MouseRightButtonDown` event as handled; see Remarks. |
| `MouseWheel` | Occurs when the user rotates the mouse wheel while the mouse pointer is over a `UIElement`, or the `UIElement` has focus. |
| `Tap` | Occurs when a Tap gesture is committed while over this `UIElement`. |
| `TextInput` | Occurs when a UI element gets text in a device-independent manner. |
| `TextInputStart` | Occurs when a UI element initially gets text in a device-independent manner. |
| `TextInputUpdate` | Occurs when text continues to be composed via an input method editor (IME). |

### Methods

| Member | Description |
|---|---|
| `AddHandler(RoutedEvent,Delegate,Boolean)` | Adds a routed event handler for a specified routed event, adding the handler to the handler collection on the current element. Specify as true to have the provided handler be invoked for routed event that had already been marked as handled by another element along the event route. <br>`routedEvent`: An identifier for the routed event to be handled. <br>`handler`: A reference to the handler implementation. <br>`handledEventsToo`: true to register the handler such that it is invoked even when the routed event is marked handled in its event data; false to register the handler with the default condition that it will not be invoked if the routed event is already marked handled. The default is false. Do not routinely ask to rehandle a routed event. For more information, see Remarks. |
| `Arrange(Rect)` | Positions child objects and determines a size for a `UIElement`. Parent objects that implement custom layout for their child elements should call this method from their layout override implementations to form a recursive layout update. <br>`finalRect`: The final size that the parent computes for the child in layout, provided as a `Rect` value. |
| `CaptureMouse` | Sets mouse capture to a `UIElement`. <br>Returns: Returns true if the object has mouse capture; otherwise, returns false. |
| `InvalidateArrange` | Invalidates the arrange state (layout) for a `UIElement`. After the invalidation, the `UIElement` will have its layout updated, which will occur asynchronously. |
| `InvalidateMeasure` | Invalidates the measurement state (layout) for a `UIElement`. |
| `Measure(Size)` | Updates the `DesiredSize` of a `UIElement`. Typically, objects that implement custom layout for their layout children call this method from their own `Size)` implementations to form a recursive layout update. <br>`availableSize`: The available space that a parent can allocate a child object. A child object can request a larger space than what is available; the provided size might be accommodated if scrolling or other resize behavior is possible in that particular container. |
| `OnCreateAutomationPeer` | When implemented in a derived class, returns class-specific `AutomationPeer` implementations for the Silverlight automation infrastructure. <br>Returns: The class-specific `AutomationPeer` subclass to return. |
| `ReleaseMouseCapture` | Removes mouse capture from a `UIElement`. After this call, typically no object holds mouse capture. |
| `RemoveHandler(RoutedEvent,Delegate)` | Removes the specified routed event handler from this `UIElement`. <br>`routedEvent`: The identifier of the routed event for which the handler is attached. <br>`handler`: The specific handler implementation to remove from the event handler collection on this `UIElement`. |
| `TransformToVisual(UIElement)` | Returns a transform object that can be used to transform coordinates from the `UIElement` to the specified object. <br>`visual`: The object to compare to the current object for purposes of obtaining the transform. <br>Returns: The transform information as an object. Call `Point)` on this object to get a practical transform. |
| `UpdateLayout` | Ensures that all positions of child objects of a `UIElement` are properly updated for layout. |

## System.Windows.VerticalAlignment

Describes how a child element is vertically positioned or stretched within a parent's layout slot.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Bottom` | The element is aligned to the bottom of the parent's layout slot. |
| `Center` | The element is aligned to the center of the parent's layout slot. |
| `Stretch` | The element is stretched to fill the entire layout slot of the parent element. |
| `Top` | The element is aligned to the top of the parent's layout slot. |

## System.Windows.Visibility

Specifies the display state of an element.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Collapsed` | Do not display the element, and do not reserve space for it in layout. |
| `Visible` | Display the element. |

## System.Windows.VisualState

Represents the visual appearance of the control when it is in a specific state.

### Properties / fields

| Member | Description |
|---|---|
| `Name` | Gets the name of the `VisualState`. <br>Returns: The name of the `VisualState`. |
| `Storyboard` | Gets or sets a `Storyboard` that defines the appearance of the control when it is the state that is represented by the `VisualState`. <br>Returns: A Storyboard that defines the appearance of the control when it is the state that is represented by the `VisualState`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VisualState` class. |

## System.Windows.VisualStateChangedEventArgs

Provides data for the `CurrentStateChanging` and `CurrentStateChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `Control` | Gets the `Control` that is transitioning states. <br>Returns: The `Control` that is transitioning states. |
| `NewState` | Gets the state the `Control` is transitioning to or has transitioned to. <br>Returns: The state the `Control` is transitioning to or has transitioned to. |
| `OldState` | Gets the state the `Control` is transitioning from or has transitioned from. <br>Returns: The state the `Control` is transitioning from or has transitioned from. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VisualStateChangedEventArgs` class. |

## System.Windows.VisualStateGroup

Contains mutually exclusive `VisualState` objects and `VisualTransition` objects that are used to go from one state to another.

### Properties / fields

| Member | Description |
|---|---|
| `CurrentState` | Gets the most recently set `VisualState` from a successful call to the `Boolean)` method. <br>Returns: The most recently set `VisualState` from a successful call to the `Boolean)` method. |
| `Name` | Gets the name of the `VisualStateGroup`. <br>Returns: The name of the `VisualStateGroup`. |
| `States` | Gets the collection of mutually exclusive `VisualState` objects. <br>Returns: The collection of mutually exclusive `VisualState` objects. |
| `Transitions` | Gets the collection of `VisualTransition` objects. <br>Returns: The collection of `VisualTransition` objects. |

### Events

| Member | Description |
|---|---|
| `CurrentStateChanged` | Occurs after a control transitions into a different state. |
| `CurrentStateChanging` | Occurs when a control begins transitioning into a different state. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VisualStateGroup` class. |

## System.Windows.VisualStateManager

Manages states and the logic for transitioning between states for controls.

### Properties / fields

| Member | Description |
|---|---|
| `CustomVisualStateManager` | Gets or sets the `VisualStateManager` that transitions between the states of a control. <br>Returns: The `VisualStateManager` that transitions between the states of a control. The default is null. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CustomVisualStateManagerProperty` | Identifies the `CustomVisualStateManager` dependency property. <br>Returns: The identifier for the `CustomVisualStateManager` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VisualStateManager` class. |
| `GetCustomVisualStateManager(FrameworkElement)` | Gets the value of the `CustomVisualStateManager` attached property. <br>`obj`: The element from which to get the `CustomVisualStateManager`. <br>Returns: The `VisualStateManager` that transitions between the states of a control. |
| `GetVisualStateGroups(FrameworkElement)` | Gets the value of the VisualStateManager.VisualStateGroups attached property. <br>`obj`: The element from which to get the VisualStateManager.VisualStateGroups. <br>Returns: The collection of `VisualStateGroup` objects that is associated with the specified object. |
| `GoToState(Control,String,Boolean)` | Transitions the control between two states. <br>`control`: The control to transition between states. <br>`stateName`: The state to transition to. <br>`useTransitions`: true to use a `VisualTransition` to transition between states; otherwise, false. <br>Returns: true if the control successfully transitioned to the new state; otherwise, false. |
| `GoToStateCore(Control,FrameworkElement,String,VisualStateGroup,VisualState,Boolean)` | Transitions a control between states. <br>`control`: The control to transition between states. <br>`templateRoot`: The root element of the control's `ControlTemplate`. <br>`stateName`: The name of the state to transition to. <br>`group`: The `VisualStateGroup` that the state belongs to. <br>`state`: The representation of the state to transition to. <br>`useTransitions`: true to use a `VisualTransition` to transition between states; otherwise, false. <br>Returns: true if the control successfully transitioned to the new state; otherwise, false. |
| `RaiseCurrentStateChanged(VisualStateGroup,VisualState,VisualState,Control)` | Raises the `CurrentStateChanged` event on the specified `VisualStateGroup`. <br>`stateGroup`: The object on which the `CurrentStateChanging` event <br>`oldState`: The state that the control transitioned from. <br>`newState`: The state that the control transitioned to. <br>`control`: The control that transitioned states. |
| `RaiseCurrentStateChanging(VisualStateGroup,VisualState,VisualState,Control)` | Raises the `CurrentStateChanging` event on the specified `VisualStateGroup`. <br>`stateGroup`: The object on which the `CurrentStateChanging` event <br>`oldState`: The state that the control is transitioning from. <br>`newState`: The state that the control is transitioning to. <br>`control`: The control that is transitioning states. |
| `SetCustomVisualStateManager(FrameworkElement,VisualStateManager)` | Sets the value of the `CustomVisualStateManager` attached property. <br>`obj`: The object on which to set the property. <br>`value`: The `VisualStateManager` that transitions between the states of a control. |

## System.Windows.VisualTransition

Represents the visual behavior that occurs when the control transitions from one state to another.

### Properties / fields

| Member | Description |
|---|---|
| `From` | Gets or sets the name of the `VisualState` to transition from. <br>Returns: The name of the `VisualState` to transition from. |
| `GeneratedDuration` | Gets or sets the amount of time it takes to move from one state to another. <br>Returns: The amount of time it takes to move from one state to another. |
| `GeneratedEasingFunction` | Gets or sets the easing function applied to the generated animations. <br>Returns: An `IEasingFunction` that is applied to the generated animations. |
| `Storyboard` | Gets or sets the `Storyboard` that occurs when the transition occurs. <br>Returns: The `Storyboard` that occurs when the transition occurs. |
| `To` | Gets or sets the name of the `VisualState` to transition to. <br>Returns: The name of the `VisualState` to transition to. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `VisualTransition` class. |

## System.Windows.Window

Represents an out-of-browser application window.

### Properties / fields

| Member | Description |
|---|---|
| `Content` | Gets or sets the root visual element that represents the contents of the window. <br>Returns: The element that represents the window contents. |
| `Height` | Gets or sets the height of the application window in pixels. <br>Returns: The height of the application window in pixels. The default is the `Height` setting in the application manifest. |
| `IsActive` | Gets a value that indicates whether the window is active. <br>Returns: true if the window is active; otherwise, false. |
| `IsVisible` | Gets a value that indicates whether the window is currently visible in the user interface. <br>Returns: A value that indicates whether the window is currently visible. |
| `Left` | Gets or sets the position of the left edge of the application window; see Remarks for restrictions on setting this property at run time. <br>Returns: The position of the left edge of the application window. The default is the `Left` setting in the application manifest. |
| `Title` | Gets or sets the window title bar text. <br>Returns: The title bar text. |
| `Top` | Gets or sets the position of the top edge of the application window; see Remarks for restrictions on setting this property at run time. <br>Returns: The position of the top edge of the application window. The default is the `Top` setting in the application manifest. |
| `TopMost` | Gets or sets a value that indicates whether the application window is always displayed in front of other windows. <br>Returns: true if the application window is topmost; otherwise, false. The default is false. |
| `Visibility` | Gets or sets a value that indicates whether the window is currently visible or collapsed. <br>Returns: A value that indicates whether the window is currently visible or collapsed. |
| `Width` | Gets or sets the width of the application window in pixels. <br>Returns: The width of the application window in pixels. The default is the `Width` setting in the application manifest. |
| `WindowState` | Gets or sets a value that indicates whether the window is maximized, minimized, or in the normal state. <br>Returns: A value that indicates the current state of the window. The default is `Normal`. |
| `WindowStyle` | Gets or sets a value that indicates the appearance of the title bar and border of the `Window`. <br>Returns: A value that indicates how the title bar and border should appear. The default is `SingleBorderWindow`. |

### Events

| Member | Description |
|---|---|
| `Closing` | Occurs when the window is about to close. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Window` class. |
| `Activate` | Attempts to activate the application window by bringing it to the foreground and setting the input focus to it. <br>Returns: true if the window was successfully activated; otherwise, false. |
| `Close` | Closes the application window unless the `Closing` event is canceled. |
| `DragMove` | In trusted applications, begins a mouse drag operation to move the window. |
| `DragResize(WindowResizeEdge)` | In trusted applications, begins a mouse drag operation to move the specified window edge or corner. <br>`resizeEdge`: The edge or corner that the user is dragging. |
| `GetWindow(DependencyObject)` | Gets the window that contains the specified `DependencyObject`. <br>`dependencyObject`: The object contained by the `Window` to get. <br>Returns: The `Window` to get. |

## System.Windows.WindowCollection

Represents a collection of `Window` instances.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of windows in the collection. <br>Returns: The number of windows in the collection. |
| `IsSynchronized` | Gets a value that indicates whether access to the collection is synchronized (thread safe). <br>Returns: Always returns false. |
| `SyncRoot` | Gets an object that can be used to synchronize access to the `WindowCollection`. <br>Returns: An object that can be used to synchronize access to the `WindowCollection`. |

### Methods

| Member | Description |
|---|---|
| `GetEnumerator` | Returns an enumerator that iterates through the collection. <br>Returns: An enumerator for the collection. |
| `System#Collections#ICollection#CopyTo(Array,Int32)` | Copies the elements of the `ICollection` to an `Array`, starting at a particular `Array` index. <br>`array`: The one-dimensional `Array` that is the destination of the elements copied from the `WindowCollection`. The array must have zero-based indexing. <br>`index`: The zero-based index in at which copying begins. |

## System.Windows.WindowResizeEdge

Defines constants that represent the edges and corners of a Silverlight out-of-browser application window.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Bottom` | The lower edge of the window. |
| `BottomLeft` | The lower-left corner of the window. |
| `BottomRight` | The lower-right corner of the window. |
| `Left` | The left edge of the window. |
| `Right` | The right edge of the window. |
| `Top` | The upper edge of the window. |
| `TopLeft` | The upper-left corner of the window. |
| `TopRight` | The upper-right corner of the window. |

## System.Windows.WindowSettings

Represents information about an out-of-browser application window.

### Properties / fields

| Member | Description |
|---|---|
| `Height` | Gets the initial window height of the application. <br>Returns: The initial window height of the application. |
| `Left` | Gets the initial position of the left edge of the out-of-browser application window when `WindowStartupLocation` is `Manual`. <br>Returns: The initial position of the left edge of the application window. |
| `Title` | Gets the full title of the out-of-browser application for display in the title bar of the application window. <br>Returns: The full title of the application. |
| `Top` | Gets the initial position of the top edge of the out-of-browser application window when `WindowStartupLocation` is `Manual`. <br>Returns: The initial position of the top edge of the application window. |
| `Width` | Gets the initial window width of the application. <br>Returns: The initial window width of the application. |
| `WindowStartupLocation` | Gets a value that indicates how the out-of-browser application window is positioned at startup. <br>Returns: A value that indicates how the application window is positioned at startup. |
| `WindowStyle` | Gets a value that indicates the appearance of the title bar and border for the out-of-browser application window. <br>Returns: A value that indicates how the title bar and border should appear. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `HeightProperty` | Identifies the `Height` dependency property. <br>Returns: The identifier for the `Height` dependency property. |
| `LeftProperty` | Identifies the `Left` dependency property. <br>Returns: The identifier for the `Left` dependency property. |
| `TitleProperty` | Identifies the `Title` dependency property. <br>Returns: The identifier for the `Title` dependency property. |
| `TopProperty` | Identifies the `Top` dependency property. <br>Returns: The identifier for the `Top` dependency property. |
| `WidthProperty` | Identifies the `Width` dependency property. <br>Returns: The identifier for the `Width` dependency property. |
| `WindowStartupLocationProperty` | Identifies the `WindowStartupLocation` dependency property. <br>Returns: The identifier for the `WindowStartupLocation` dependency property. |
| `WindowStyleProperty` | Identifies the `WindowStyle` dependency property. <br>Returns: The identifier for the `WindowStyle` dependency property. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `WindowSettings` class. |

## System.Windows.WindowStartupLocation

Defines constants that indicate how an out-of-browser application window is positioned at startup.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CenterScreen` | The application window is centered in the screen, and the `Top` and `Left` settings are ignored. |
| `Manual` | The application window is positioned according to the `Top` and `Left` settings. |

## System.Windows.WindowState

Defines constants that indicate the state of an out-of-browser application window.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Maximized` | The application window is maximized to occupy the entire client area of the screen. |
| `Minimized` | The application window is minimized to the taskbar. |
| `Normal` | The application window is in its normal state, occupying screen space based on its `Height` and `Width` values. |

## System.Windows.WindowStyle

Defines constants that indicate the appearance of the title bar and border of an out-of-browser application window.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BorderlessRoundCornersWindow` | The window does not display a title bar or border, and the window corners are rounded. |
| `None` | The window does not display a title bar or border. |
| `SingleBorderWindow` | The window displays a title bar and border. |

## System.Xaml.IMarkupExtension`1

Provides an interface basis for XAML markup extension implementations that can be supported by Silverlight XAML processors.

### Methods

| Member | Description |
|---|---|
| `ProvideValue(IServiceProvider)` | Returns an object that is provided as the value of the target property for the markup extension. <br>`serviceProvider`: A service provider helper that can provide services for the markup extension. <br>Returns: The value to set on the property where the extension is applied.. |

## System.Xaml.IRootObjectProvider

Describes a service that can return the root object of markup being parsed.

### Properties / fields

| Member | Description |
|---|---|
| `RootObject` | Gets the root object from markup or from an object graph. <br>Returns: The root object. |
