<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class SoftwareTriggerFm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.btnOpen = New System.Windows.Forms.Button()
        Me.btnClose = New System.Windows.Forms.Button()
        Me.btnTrigger = New System.Windows.Forms.Button()
        Me.pbImage = New System.Windows.Forms.PictureBox()
        CType(Me.pbImage, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'btnOpen
        '
        Me.btnOpen.Location = New System.Drawing.Point(81, 13)
        Me.btnOpen.Name = "btnOpen"
        Me.btnOpen.Size = New System.Drawing.Size(75, 23)
        Me.btnOpen.TabIndex = 0
        Me.btnOpen.Text = "Open"
        Me.btnOpen.UseVisualStyleBackColor = True
        '
        'btnClose
        '
        Me.btnClose.Location = New System.Drawing.Point(251, 13)
        Me.btnClose.Name = "btnClose"
        Me.btnClose.Size = New System.Drawing.Size(75, 23)
        Me.btnClose.TabIndex = 1
        Me.btnClose.Text = "Close"
        Me.btnClose.UseVisualStyleBackColor = True
        '
        'btnTrigger
        '
        Me.btnTrigger.Location = New System.Drawing.Point(436, 12)
        Me.btnTrigger.Name = "btnTrigger"
        Me.btnTrigger.Size = New System.Drawing.Size(75, 23)
        Me.btnTrigger.TabIndex = 2
        Me.btnTrigger.Text = "Trigger"
        Me.btnTrigger.UseVisualStyleBackColor = True
        '
        'pbImage
        '
        Me.pbImage.Location = New System.Drawing.Point(13, 47)
        Me.pbImage.Name = "pbImage"
        Me.pbImage.Size = New System.Drawing.Size(605, 428)
        Me.pbImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.pbImage.TabIndex = 3
        Me.pbImage.TabStop = False
        '
        'SoftwareTriggerFm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(630, 489)
        Me.Controls.Add(Me.pbImage)
        Me.Controls.Add(Me.btnTrigger)
        Me.Controls.Add(Me.btnClose)
        Me.Controls.Add(Me.btnOpen)
        Me.MaximizeBox = False
        Me.Name = "SoftwareTriggerFm"
        Me.Text = "SoftwareTrigger"
        CType(Me.pbImage, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents btnOpen As System.Windows.Forms.Button
    Friend WithEvents btnClose As System.Windows.Forms.Button
    Friend WithEvents btnTrigger As System.Windows.Forms.Button
    Friend WithEvents pbImage As System.Windows.Forms.PictureBox

End Class
