# Warrior
 基于GAS的ActionRPG，使用PuerTS进行热更新

## 功能
实现了以下功能（主要依据历史提交记录）：
* Spawn Weapon
* Melee Combo System
    * Weapon Combat Component && Binding Input
    * Armed Locomotion
    * Attack Combo Chain
* Deal Damage
    * Add Enemy Characters
    * Init Attributes
    * Hit Detection
      * Toggle Weapon Collision
      * Hit Check
      * Process Hit && Notify Result
    * Apply Damage: Custom Calculation
      * Make Gameplay Effect Spec Handle
      * Apply Handle To Target
      * Gameplay Effect Execution Calculation
        * Capture Relevant Attributes
        * Retrieve Attribute Info
        * Meta Attribute(DamageTaken) 
      * Modify Health Attribute
    * Combat Feedback
      * Hit React
      * Hit/Sound FX
      * Camera Shake
      * Death
    * Notify UI
