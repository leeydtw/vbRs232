Imports System.ComponentModel
Imports System.IO.Ports

Public Class Form1
    Delegate Sub SetTextCallback(ByVal InputString As String)
    '宣告一個委派類別
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        cmbPort.Items.Clear()
        For Each PortName As String In SerialPort.GetPortNames
            cmbPort.Items.Add(PortName)
        Next
        cmbPort.SelectedIndex = 0
    End Sub

    Private Sub btnOpenPort_Click(sender As Object, e As EventArgs) Handles btnOpenPort.Click
        If SerialPort1.IsOpen Then
            BkWorker.CancelAsync()  '取消背景作業
            SerialPort1.Close()
            btnOpenPort.ImageKey = "open"
            btnOpenPort.Text = "開啟"
            btnSend.Enabled = False
        Else
            SerialPort1.PortName = cmbPort.SelectedItem.ToString
            SerialPort1.BaudRate = 9600
            SerialPort1.Parity = Parity.None
            SerialPort1.DataBits = 8
            SerialPort1.StopBits = StopBits.One
            SerialPort1.Open()
            btnOpenPort.ImageKey = "close"
            btnOpenPort.Text = "關閉"
            btnSend.Enabled = True
            BkWorker.WorkerSupportsCancellation = True
            BkWorker.RunWorkerAsync()   '開啟背景作業
        End If
    End Sub

    Private Sub btnSend_Click(sender As Object, e As EventArgs) Handles btnSend.Click
        SerialPort1.Write(txtSend.Text)
    End Sub

    Private Sub DisplayText(ByVal comData As String)    '委派副程式
        If Me.txtReceive.InvokeRequired Then
            Dim d As New SetTextCallback(AddressOf showString)  '委派物件d參數是字串
            Me.Invoke(d, New Object() {comData})    '字串就是傳送的數據
        Else
            txtReceive.Text = ""
            txtReceive.Text = comData
        End If
    End Sub

    Private Sub GetRS232Data()
        Dim InString As String
        InString = ""
        If Not SerialPort1.IsOpen Then Exit Sub
        Try
            InString = SerialPort1.ReadExisting()
            If InString.Length = 0 Then
                Exit Sub
            Else
                DisplayText(InString)
            End If
        Catch ex As Exception
            MessageBox.Show("讀取錯誤 : " + ex.ToString, "錯誤通知", MessageBoxButtons.OK, MessageBoxIcon.Exclamation)
        End Try
    End Sub

    '當RunWorkerAsync方法呼叫時會觸發此事件但只會執行一次
    Private Sub BkWorker_DoWork(sender As Object, e As DoWorkEventArgs) Handles BkWorker.DoWork
        GetRS232Data()
        Application.DoEvents()
        If BkWorker.CancellationPending Then
            e.Cancel = True
        End If
    End Sub

    '當DoWork事件被執行完畢後會觸發此事件
    Private Sub BkWorker_RunWorkerCompleted(sender As Object, e As RunWorkerCompletedEventArgs) Handles BkWorker.RunWorkerCompleted
        If Not BkWorker.CancellationPending Then
            BkWorker.RunWorkerAsync()   '開啟背景作業
        End If
    End Sub

    Private Sub showString(ByVal comData As String)
        txtReceive.Text = ""
        txtReceive.Text = comData
    End Sub
End Class
