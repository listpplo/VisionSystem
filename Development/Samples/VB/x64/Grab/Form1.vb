Public Class Form1
    REM :Camera device
    Dim m_dev As ThridLibray.IDevice

    Private Sub Form1_Shown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Shown
        btnClose.Enabled = False
    End Sub

    REM :Handle for camera open
    Private Sub OnCameraOpen(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Invoke(New Action(Sub()
                              btnOpen.Enabled = False
                              btnClose.Enabled = True
                          End Sub))
    End Sub

    REM :Handle for camera close
    Private Sub OnCameraClose(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Invoke(New Action(Sub()
                              btnOpen.Enabled = True
                              btnClose.Enabled = False
                          End Sub))
    End Sub

    REM :Handle for connection lost
    Private Sub OnCameraLoss(ByVal sender As System.Object, ByVal e As System.EventArgs)
        If Not m_dev Is Nothing Then
            If m_dev.IsOpen Then
                m_dev.Close()
                m_dev = Nothing
            End If
        End If

        Invoke(New Action(Sub()
                              btnOpen.Enabled = True
                              btnClose.Enabled = False
                          End Sub))
    End Sub

    REM :Image graphics
    Dim _g As Graphics = Nothing

    REM :Handle for grabbing
    Private Sub OnImageGrabbed(ByVal sender As System.Object, ByVal e As ThridLibray.GrabbedEventArgs)
        REM :Create bitmap form raw data
        Dim bitmap As Bitmap = e.GrabResult.ToBitmap(False)

        REM :Mark sure operator run on ui thread
        If pbImage.InvokeRequired Then
            Try
                                                          REM :Init graphics
                                                          If _g Is Nothing Then
                                                              _g = pbImage.CreateGraphics
                                                          End If

                                                          REM :Display bitmap
                                                          _g.DrawImage(bitmap,
                                                                       New Rectangle(0, 0, pbImage.Width, pbImage.Height),
                                                                       New Rectangle(0, 0, bitmap.Width, bitmap.Height),
                                                                       GraphicsUnit.Pixel)
                                                          bitmap.Dispose()
            Catch ex As Exception
                Throw New Exception(ex.Message, ex)
            End Try
        End If
    End Sub


    Private Sub btnOpen_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOpen.Click
        Try
            REM :Enum all device
            Dim DeviceList As System.Collections.Generic.List(Of ThridLibray.IDeviceInfo) =
                (ThridLibray.Enumerator.EnumerateDevices())

            If DeviceList.Count > 0 Then
                REM :Get device at first index 
                m_dev = ThridLibray.Enumerator.GetDeviceByIndex(0)

                REM :Add some handler for connection
                AddHandler m_dev.CameraOpened, AddressOf OnCameraOpen
                AddHandler m_dev.CameraClosed, AddressOf OnCameraClose
                AddHandler m_dev.ConnectionLost, AddressOf OnCameraLoss

                REM :Open Device
                If Not m_dev.Open Then
                    MessageBox.Show("Open Camera Failed")
                    Return
                End If

                REM :Set Pixelformat to Mono8
                Using ep As ThridLibray.IEnumParameter = m_dev.ParameterCollection(ThridLibray.ParametrizeNameSet.ImagePixelFormat)
                    If Not ep Is Nothing Then
                        ep.SetValue("Mono8")
                        ep.Dispose()
                    End If
                End Using

                REM :Close trigger mode
                m_dev.TriggerSet.Close()

                REM :Add grab handle
                AddHandler m_dev.StreamGrabber.ImageGrabbed, AddressOf OnImageGrabbed

                REM :Start grabbing
                If Not m_dev.GrabUsingGrabLoopThread() Then
                    MessageBox.Show("Open Grabing Failed")
                    Return
                End If
            Else
                MessageBox.Show("No Available Device")
            End If
        Catch ex As Exception
            Throw New Exception(ex.Message, ex)
        End Try
    End Sub

    Private Sub btnClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnClose.Click
        Try
            REM :Close camera
            If Not m_dev Is Nothing Then
                m_dev.Close()
            End If
        Catch ex As Exception
            Throw New Exception(ex.Message, ex)
        End Try
    End Sub

    Protected Overrides Sub OnClosed(ByVal e As System.EventArgs)
        If Not m_dev Is Nothing Then
            m_dev.Dispose()
            m_dev = Nothing
        End If

        If Not _g Is Nothing Then
            _g.Dispose()
            _g = Nothing
        End If
        MyBase.OnClosed(e)
    End Sub
End Class
