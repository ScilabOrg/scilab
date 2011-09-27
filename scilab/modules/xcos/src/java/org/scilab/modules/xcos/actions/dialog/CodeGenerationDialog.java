/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions.dialog;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.codegeneration.CodegenerationTargets;
import org.scilab.modules.xcos.codegeneration.Options;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;


/**
 * Dialog associated with the {@link SetupAction}.
 * 
 * Note that this dialog break the Data Abstraction Coupling metric because of
 * the numbers of graphical components involved in the GUI creation. For the
 * same reason (GUI class), constants are not used on this code.
 */
//CSOFF: ClassDataAbstractionCoupling
//CSOFF: ClassFanOutComplexity
//CSOFF: MagicNumber
public class CodeGenerationDialog extends JFrame {

	//JFrame objects
    private JButton okButton;
    private JButton cancelButton;
    private JLabel targetLabel;
    private JLabel descriptionLabel;
    private JLabel descriptiondataLabel;
    private JComboBox targetList;
    private JPanel targetselectionPanel;
    private JPanel[] optionsPanel;
    private JTextField[] optionsPanelTextField;
    private JButton[] optionsPanelButton;
	
	//Data objects
	private CodegenerationTargets targets;
	private ScilabGraph scilabGraph;
	
	/**
	 * Instanciate a new dialog.
	 * 
	 * @param parent the current selected graph component
	 * @param scilabGraph the current Xcos diagram
	 * @param numberOfTargets Necessary for CodeGenerationDialog to allocate memory for all targets
	 */
	public CodeGenerationDialog(Component parent, ScilabGraph argScilabGraph) {
		super();
		
		//Initialize global attributes
		targets = new CodegenerationTargets();
		scilabGraph = argScilabGraph;
		
		//Initialize the dialog
		initComponents();
	}
	
	/**
	 * Initialize the dialog components.
	 * 
	 * As this method perform a complex initialization, It doesn't pass NCSS. 
	 */
	//CSOFF: JavaNCSS
	private void initComponents() {
		/* Initialize JFrame components data */

		//create list of targets. Resolve target name from CodeGenerationTarget[] array
		String[] targetString = new String[targets.getNumberOfTargets()];
		
		int totalNumberOfBrowseButtons=0; int totalNumberOfTextFields=0;
		for(int i=0; i<targets.getNumberOfTargets(); i++) {
			//Fill the string array with targetname's
			targetString[i] = targets.getCodegenerationTarget(i).getName();
			
			//Calculate the total number of buttons required for initialization
			totalNumberOfBrowseButtons += targets.getCodegenerationTarget(i).getNumberOfDirectoryTypeOptions();
			
			//Calculate the total number of JTextFields required for initialization
			totalNumberOfTextFields += targets.getCodegenerationTarget(i).getNumberOfOptions();
		}

		/* Initialize JFrame components layout */
		optionsPanel = new JPanel[targets.getNumberOfTargets()];
		optionsPanelButton = new JButton[totalNumberOfBrowseButtons];
		optionsPanelTextField = new JTextField[totalNumberOfTextFields];
		
		GridBagConstraints gridBagConstraints;

	    okButton = new JButton(XcosMessages.OK);
	    cancelButton = new JButton(XcosMessages.CANCEL);
	    targetLabel = new JLabel("Target:");
	    descriptionLabel = new JLabel("Description:");
	    descriptiondataLabel = new JLabel();
	    targetList = new JComboBox(targetString );
	    targetselectionPanel = new JPanel();
	    
		Icon scilabIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
		Image imageForIcon = ((ImageIcon) scilabIcon).getImage();
		setLayout(new GridBagLayout());
		setIconImage(imageForIcon);

		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setTitle("Code generation selections");

        //setResizable(false);
        getContentPane().setLayout(new GridBagLayout());

	    targetselectionPanel.setBorder(BorderFactory.createTitledBorder(" Target selection ") );
	    targetselectionPanel.setLayout(new GridBagLayout());

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.ipadx = 150;
        gridBagConstraints.anchor = GridBagConstraints.EAST;
        gridBagConstraints.insets = new Insets(32, 0, 0, 0);
        targetselectionPanel.add(targetLabel, gridBagConstraints);

        targetList.setVerifyInputWhenFocusTarget(false);
		targetList.setSelectedIndex(0);

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 1;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.gridwidth = 2;
        gridBagConstraints.ipadx = 394;
        gridBagConstraints.anchor = GridBagConstraints.WEST;
        gridBagConstraints.insets = new Insets(21, 12, 0, 18);
        targetselectionPanel.add(targetList, gridBagConstraints);
        
        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.ipadx = 150;
        gridBagConstraints.anchor = GridBagConstraints.EAST;
        gridBagConstraints.insets = new Insets(12, 0, 18, 0);
        targetselectionPanel.add(descriptionLabel, gridBagConstraints);
        
        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 1;
        gridBagConstraints.gridy = 1;
        gridBagConstraints.anchor = GridBagConstraints.WEST;
        gridBagConstraints.insets = new Insets(12, 12, 18, 0);
        targetselectionPanel.add(descriptiondataLabel, gridBagConstraints);

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.gridwidth = 2;
        gridBagConstraints.insets = new Insets(12, 12, 0, 12);
        getContentPane().add(targetselectionPanel, gridBagConstraints);
        
        /*
         * Beginning of optional elements
         * Create as much option panels as there are targets
         */
        Options targetOptions;
        int j=0; int optionsOffset=0; int optionsDirectoryTypeOffset=0;
        for(int i=0; i<targets.getNumberOfTargets(); i++)
        {
		    optionsPanel[i] = new JPanel();
		    optionsPanel[i].setBorder(BorderFactory.createTitledBorder(" Target specific options ") );
		    optionsPanel[i].setLayout(new GridBagLayout());
		    
		    //Get the options for the target selected
		    targetOptions = targets.getCodegenerationTarget(i).getOptions();
		    for(j=0; j<targetOptions.getNumberOfOptions(); j++)
		    {
		    	//Add the option name to the panel as label
		        gridBagConstraints = new GridBagConstraints();
		        gridBagConstraints.gridx = 0;
		        gridBagConstraints.gridy = j;
		        gridBagConstraints.ipadx = 150;
		        gridBagConstraints.anchor = GridBagConstraints.EAST;
		        gridBagConstraints.insets = new Insets(18, 0, 0, 0);
		    	optionsPanel[i].add(new JLabel(targetOptions.getSpecifiedOption(j).getName() ), gridBagConstraints);
		    	
		    	optionsPanelTextField[optionsOffset] = new JTextField();
		    	
		    	//Add the elements according to the option type to the panel
		    	if(targetOptions.getSpecifiedOption(j).getType()=="TEXT") {
			        gridBagConstraints = new GridBagConstraints();
			        gridBagConstraints.gridx = 1;
			        gridBagConstraints.gridy = j;
			        gridBagConstraints.gridwidth = 2;
			        gridBagConstraints.ipadx = 408;
			        gridBagConstraints.anchor = GridBagConstraints.WEST;
			        gridBagConstraints.weightx = 1.0;
			        gridBagConstraints.insets = new Insets(21, 18, 0, 18);
			        optionsPanel[i].add(optionsPanelTextField[optionsOffset], gridBagConstraints);
		    	} else {
			        gridBagConstraints = new GridBagConstraints();
			        gridBagConstraints.gridx = 1;
			        gridBagConstraints.gridy = j;
			        gridBagConstraints.ipadx = 303;
			        gridBagConstraints.anchor = GridBagConstraints.WEST;
			        gridBagConstraints.weightx = 1.0;
			        gridBagConstraints.insets = new Insets(13, 18, 0, 0);
			        optionsPanel[i].add(optionsPanelTextField[optionsOffset], gridBagConstraints);
			        
			        optionsPanelButton[optionsDirectoryTypeOffset] = new JButton("Browse");
			        
			        gridBagConstraints = new GridBagConstraints();
			        gridBagConstraints.gridx = 2;
			        gridBagConstraints.gridy = j;
			        gridBagConstraints.ipadx = 18;
			        gridBagConstraints.anchor = GridBagConstraints.WEST;
			        gridBagConstraints.insets = new Insets(12, 12, 18, 0);
			        optionsPanel[i].add(optionsPanelButton[optionsDirectoryTypeOffset], gridBagConstraints);
			        
			        optionsDirectoryTypeOffset++;
		    	}
		    	optionsOffset++;
		    }
		    
	        //Only show the initially selected panel
	        if(i==targetList.getSelectedIndex()) {
	        	optionsPanel[i].setVisible(true);
	        } else {
	        	optionsPanel[i].setVisible(false);
	        }
		    
		    //Add the optionsPanel to the pane
	        gridBagConstraints = new GridBagConstraints();
	        gridBagConstraints.gridx = 0;
	        gridBagConstraints.gridy = 1;
	        gridBagConstraints.gridwidth = 2;
	        gridBagConstraints.insets = new Insets(12, 12, 0, 12);
	        getContentPane().add(optionsPanel[i], gridBagConstraints);
		    
		    /*
		     * TODO: Think about default JFieldText text.
		     * 
		    blocknameField = new JTextField(scilabGraph.getTitle() );
		    directoryField = new JTextField(System.getenv("USERPROFILE") + "/" + scilabGraph.getTitle());
		    */
		    
        
        }
        //End of optional elements

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 2;
        gridBagConstraints.ipadx = 62;
        gridBagConstraints.insets = new Insets(12, 398, 12, 0);
        getContentPane().add(okButton, gridBagConstraints);

        gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridx = 1;
        gridBagConstraints.gridy = 2;
        gridBagConstraints.ipadx = 37;
        gridBagConstraints.insets = new Insets(12, 6, 12, 12);
        getContentPane().add(cancelButton, gridBagConstraints);

		/* Setup JFrame components */
		targetList.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					//Show the description of the currently selected target
					descriptiondataLabel.setText(targets.getCodegenerationTarget(targetList.getSelectedIndex()).getDescription() );
					
					//hide all targets
					for(int i=0; i<targets.getNumberOfTargets(); i++) {
						getContentPane().getComponent(i + 1).setVisible(false);
					}
					
					//Show the optionspanel of the currently selected target	
					getContentPane().getComponent(targetList.getSelectedIndex() + 1).setVisible(true);
					
					//rebuild the dialog
					pack();
					}
			});
		//Initial load for the target description label
		descriptiondataLabel.setText(targets.getCodegenerationTarget(targetList.getSelectedIndex()).getDescription() );
		

		installActionListeners();	
		pack();
		setLocationRelativeTo(null);
		}
	//CSON: JavaNCSS

	/**
	 * Install the action listeners on the buttons
	 * 
	 * @param cancelButton the cancel button (Cancel)
	 * @param okButton the OK button (Validate)
	 * @param defaultButton the default button (Reset)
	 * @param setContextButton the context button (Shortcut to set context)
	 */
	private void installActionListeners() {
		
		cancelButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				//Release CodeGenerationDialog memory resources
				dispose();
			}
		});

		//Install Browsebutton actionlisteners
        for(int i=0; i<optionsPanelButton.length; i++) {
        optionsPanelButton[i].addActionListener(new ActionListener() {        	
			@Override			
			public void actionPerformed(ActionEvent e) {
			    JFileChooser fileChooser = new JFileChooser();
			    int optionsOffset=0; 
			    int selectedTextField=-1;
			    int optionsDirectoryTypeOffset=0;
			    
			    //retrieve the index of the JTextField to write to
			    
			    //Loop through all targets
			    for(int i=0; i<targets.getNumberOfTargets(); i++) {
			    	//Loop through all objects within that target
			    	for(int j=0; j<targets.getCodegenerationTarget(i).getNumberOfOptions(); j++) {
			    		if(targets.getCodegenerationTarget(i).getOptions().getSpecifiedOption(j).getType()=="DIRECTORY") {
			    			if(e.getSource().equals(optionsPanelButton[optionsDirectoryTypeOffset]) ) {
			    				selectedTextField=optionsOffset;
			    			}
			    			optionsDirectoryTypeOffset++;
			    		}
			    		optionsOffset++;
			    	}
			    }
			    
			    //open the open dialog
			    fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			    fileChooser.setMultiSelectionEnabled(false);
			    int returnVal = fileChooser.showOpenDialog(null);

		        if (returnVal == JFileChooser.APPROVE_OPTION) {
			    //Write the directory here
			    optionsPanelTextField[selectedTextField].setText(fileChooser.getSelectedFile().getPath() );
		        }
			}
		});
        }
		
		okButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				String macroURL = ScilabConstants.SCI.getAbsoluteFile() + XcosConstants.XCOS_CODEGENERATION
						+ File.separator 
						+ targets.getCodegenerationTarget(getSelectedTargetIndex()).getMacroDirectory() 
						+ File.separator
						+ targets.getCodegenerationTarget(getSelectedTargetIndex()).getMacroName()
						+ ".bin";
				
				//Check whether the selected macro exists
				if( !(new File(macroURL)).exists())
				{
					JOptionPane.showMessageDialog(null, "The codegeneration macro doesn't exist.\n\n" +
							macroURL, 
							"Codegenerating error",  JOptionPane.WARNING_MESSAGE);
					
					//Release CodeGenerationDialog memory resources
					dispose();
					return;
				}
				
				if (((JButton) e.getSource()).hasFocus()) {
					Object selectedObj = scilabGraph.getSelectionCell();
				
					//make sure that the selected object is a superblock
					if (!(selectedObj instanceof SuperBlock)) {
						((XcosDiagram) scilabGraph).error(XcosMessages.ERROR_GENERATING_C_CODE);
						dispose();
						return;
					}
			
					((XcosDiagram) scilabGraph).info(XcosMessages.GENERATING_C_CODE);
				
					final SuperBlock block = (SuperBlock) selectedObj;
				
					try {
						/*
						 * Prepare data
						 */
						final File tempOutput = new File(FileUtils.createTempFile());
						final File tempInput = new File(FileUtils.createTempFile());
						
					    /*
					     * Export data
					     */
						new H5RWHandler(tempOutput).writeBlock(block);
						new H5RWHandler(tempOutput).writeScilabList( getOptionsList(getSelectedTargetIndex()) );
						
						final ActionListener callback = new ActionListener() {
							@Override
							public void actionPerformed(ActionEvent arg0) {
								if (!tempInput.exists()) {
									((XcosDiagram) scilabGraph).info(XcosMessages.EMPTY_INFO);
									return;
								}
								
								XcosDiagram graph = block.getParentDiagram();
								if (graph == null) {
									block.setParentDiagram(Xcos.findParent(block));
									graph = block.getParentDiagram();
									LogFactory.getLog(getClass()).error("Parent diagram was null");
								}
								
								graph.getModel().beginUpdate();
								doAction(block, tempInput.getAbsolutePath());
								graph.getModel().endUpdate();
								
								graph.getView().clear(block, true, false);
								graph.getView().validate();
								
								//delete(tempOutput);
								//delete(tempInput);
								
								((XcosDiagram) scilabGraph).info(XcosMessages.EMPTY_INFO);
							}
						};
						
						//load macro of selected codegeneration target
						String cmd = buildCall("load", macroURL );

					    /*
					     * Prepare command for selected target and callback
					     */
						cmd += buildCall(targets.getCodegenerationTarget(getSelectedTargetIndex()).getMacroName(),
								tempOutput.getAbsolutePath(),
								tempInput.getAbsolutePath());
						
						asynchronousScilabExec(callback, cmd );
						
					} catch (IOException ex) {
						LogFactory.getLog(CodeGenerationDialog.class).error(ex);
					    ((XcosDiagram) scilabGraph).info(XcosMessages.EMPTY_INFO);
					}  catch (InterpreterException ex) {
						LogFactory.getLog(CodeGenerationDialog.class).error(ex);
					}
					
					//Release CodeGenerationDialog memory resources
					dispose();
			    }
			}
		});
		

	}
	
    /**
     * Callback function
     * 
     * Read the block from the scilab 
     * 
     * @param block The block we are working on
     * @param tempInput Input file
     */
    
    public int getSelectedTargetIndex() {
    	return targetList.getSelectedIndex();
    }
    
    /**
     * Callback function
     * 
     * Read the block from the scilab 
     * 
     * @param block The block we are working on
     * @param tempInput Input file
     */
    private static void doAction(final SuperBlock block,
			final String tempInput) {
    	try {
	    BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
	    block.updateBlockSettings(modifiedBlock);
	    block.setInterfaceFunctionName(modifiedBlock.getInterfaceFunctionName());
	    block.setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
	    block.setSimulationFunctionType(modifiedBlock.getSimulationFunctionType());
	    block.setChild(null);
	    
	    block.setStyle(block.getStyle() + ";blockWithLabel");
	    block.setValue(block.getSimulationFunctionName());
	    BlockPositioning.updateBlockView(block);
    	} catch (ScicosFormatException e) {
			LogFactory.getLog(CodeGenerationAction.class).error(e);
		}
	}
    
	/**
	 * Returns a ScilabList containing the codegeneration options
	 * @param targetIndex
	 * @return codegeneration options
	 */
    public ScilabList getOptionsList(int targetIndex) {
    	ScilabList options = new ScilabList();
    	int offset=0;
    	
    	//Calculate the offset of the selected target options within
    	//the optionsPanelTextField array
    	for(int i=0; i<targetIndex; i++) {
    		offset += targets.getCodegenerationTarget(i).getNumberOfOptions();
    	}
    	
    	//Add the options to the ScilabList
		for(int i=0; i<targets.getCodegenerationTarget(targetIndex).getNumberOfOptions(); i++) {
			options.add( new ScilabString(
					optionsPanelTextField[offset + i].getText()
						) );
		}
		
    	return options;
    }
    
}
//CSON: ClassDataAbstractionCoupling
//CSON: ClassFanOutComplexity
//CSON: MagicNumber
