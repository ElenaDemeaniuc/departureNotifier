using System;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Microsoft.AspNetCore.WebUtilities;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text;
using System.Net.Sockets;

namespace WebAPIClient
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();
        static List<string> Data;
        static readonly object LockObject = new object();

        private static int linenumber = 0;
        private const int port = 5000;
        private const string HostName = "192.168.0.108";
        static void Connect(String server, String message)
        {
            try
            {
                TcpClient client = new TcpClient(server, port);
                Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);
                NetworkStream stream = client.GetStream();
                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", message);
                stream.Close();
                client.Close();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }

            Console.WriteLine("\n Press Enter to continue...");
            Console.Read();
        }




        private static async Task ProcessRepositories()
        {
            DateTimeOffset CurrentTime = DateTime.Now;
            string DateToday = CurrentTime.ToString("yyyy.MM.dd");
            string TimeNow = CurrentTime.ToString("HH:mm:ss");
            int TimeNowMillis = StringToMillis(DateToday, TimeNow);

            int StringToMillis(string InputDateToday, string inputTime)
            {
                string valueString = $"{InputDateToday}";
                string valueString2 = $"{InputDateToday} {inputTime}";
                DateTimeOffset valueDateTime = Convert.ToDateTime(valueString); //ofset because GMT    
                DateTimeOffset valueDateTime2 = Convert.ToDateTime(valueString2);
                TimeSpan PassedTime = valueDateTime2.Subtract(valueDateTime);
                int valueInMillisecons = (int)PassedTime.TotalMilliseconds;
                return valueInMillisecons;
            }

            Dictionary<string, int> homeToStation = new Dictionary<string, int>();
            var array = File.ReadAllLines("StationText.txt");
            for (var i = 0; i < array.Length; i++)
            {
                string[] subs = array[i].Split(':');
                if (String.IsNullOrEmpty(subs[1]))
                    Console.WriteLine("File content error, time value is missing");
                try
                {
                    int a = int.Parse(subs[1]);
                    homeToStation.Add(subs[0], a); //subs[0] should be in hungarian letters. Copy-paste from website
                }
                catch (FormatException)
                {
                    Console.WriteLine($"Unable to parse '{subs[1]}' for '{subs[0]}'");
                }
            }

            StringBuilder sb1 = new StringBuilder();
            sb1.AppendLine($"{TimeNowMillis}");
            Data.Add(sb1.ToString());

            var array1 = File.ReadAllLines("CitiesText.txt");
            for (var i = 0; i < array1.Length; i++)
            {
                for (var j = 0; j < array1.Length; j++)
                {
                    if (i != j)
                    {
                        string stationstart = array1[i];
                        string stationend = array1[j];

                        var query = new Dictionary<string, string>()
                        {
                            ["from"] = stationstart,
                            ["to"] = stationend,
                            ["date"] = DateToday       //["date"] = "2022.05.23" correct date format         
                        };
                        var uri = QueryHelpers.AddQueryString("https://apiv2.oroszi.net/elvira", query); //building uri
                        var streamTask = client.GetStreamAsync(uri);
                        var schedule = await JsonSerializer.DeserializeAsync<Schedule>(await streamTask); //deserialize Json

                        List<Train> listOfTrains = new List<Train>();

                        foreach (var tr in schedule.timetable)
                        {
                            int starting = StringToMillis(DateToday, tr.starttime);
                            int ending = StringToMillis(DateToday, tr.destinationtime);
                            int minutetemp;
                            int leaving = 0;
                            bool hasValue = homeToStation.TryGetValue(tr.start, out minutetemp);
                            if (hasValue)
                            {
                                leaving = starting - minutetemp * 60000;
                            }
                            else
                            {
                                Console.WriteLine("Key not present");
                            }
                            if (starting > TimeNowMillis + minutetemp * 60000 + 60000 && starting < TimeNowMillis + 60000 * 180) //current time plus 1 minute
                                listOfTrains.Add(new Train(tr, leaving, array1[i].Trim(new char[] { '*' }), array1[j].Trim(new char[] { '*' })));
                        }

                        StringBuilder sb = new StringBuilder();

                        Train[] arrayOfTrains = listOfTrains.ToArray();

                        foreach (var trainn in arrayOfTrains)
                        {
                            //Console.WriteLine($"{trainn.startCity} to {trainn.endCity} Start: {trainn.startStation} at {trainn.startTime}, End: {trainn.endStation} at {trainn.endTime}, {trainn.timeOnRoad}, Leave home at: {trainn.timeToLeaveHome}");

                            sb.AppendLine($"{trainn.startCity};{trainn.endCity};{trainn.startTime};{trainn.timeToLeaveHome};{trainn.startStation};{trainn.endStation};{trainn.endTime};{trainn.timeOnRoad}");
                            linenumber = linenumber + 1;
                            // Data.Add($"{trainn.startCity};{trainn.endCity};{trainn.startTime};{trainn.timeToLeaveHome};{trainn.startStation};{trainn.endStation};{trainn.endTime};{trainn.timeOnRoad}"));
                        }
                        lock (LockObject) { Data.Add(sb.ToString()); }

                    }
                }
            }
        }

        static async Task Main(string[] args)
        {
            lock (LockObject) { Data = new List<string>(); }
            await ProcessRepositories();
            //Console.WriteLine(linenumber);
            Data.Insert(0, linenumber.ToString() + "\n");
            // foreach (var i in Data)
            // {
            //     Console.WriteLine(i);
            // }
            StringBuilder datastring = new StringBuilder();
            foreach (var j in Data)
            {
                datastring.Append(j);
            }
            Console.WriteLine(datastring);
            String datastring2 = datastring.ToString();
            String datastring3 = datastring2.Replace("É", "E").Replace("é", "e").Replace("õ", "o").Replace("á", "a").Replace("ö", "o").Replace("ó", "o");
            //send over tcp  
            Connect(HostName, datastring3);
        }
    }
}

