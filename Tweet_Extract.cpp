/**
 * @file tweet_extract.cpp
 * @author Elizabeth Dorfman
 * @brief Fetches tweets from Twitter API and outputs them into a styled HTML page for analysis.
 *
 * This program reads a Bearer Token from a `.env` file, queries the Twitter API v2
 * for recent tweets based on a keyword, processes the results, and exports them
 * into an HTML file. This is useful for further sentiment or word cloud analysis.
 */

 #include <iostream>
 #include <string>
 #include <vector>
 #include <algorithm>
 #include <fstream>
 #include <sstream>

 #define CPPHTTPLIB_OPENSSL_SUPPORT
 #include <cpp-httplib/httplib.h>
 #include <nlohmann/json.hpp>

 /**
  * @brief Reads the Bearer Token from a `.env` file.
  *
  * Looks for a line starting with `BEARER_TOKEN=` and extracts the token value,
  * removing quotes and whitespace.
  *
  * @return The bearer token as a std::string. Returns an empty string if not found.
  * @author Elizabeth Dorfman
  */
 std::string read_bearer_token() {
	 std::ifstream file(".env");
	 std::string line;
	 while (std::getline(file, line)) {
		 if (line.find("BEARER_TOKEN=") == 0) {
			 std::string token = line.substr(13);
			 token.erase(std::remove(token.begin(), token.end(), '"'), token.end());
			 token.erase(std::remove(token.begin(), token.end(), '\''), token.end());
			 token.erase(std::remove(token.begin(), token.end(), '\r'), token.end());
			 token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());
			 token.erase(0, token.find_first_not_of(" \t"));
			 token.erase(token.find_last_not_of(" \t") + 1);
			 return token;
		 }
	 }
	 return "";
 }

 /**
  * @struct Tweet
  * @brief Represents a single tweet's relevant data.
  * @author Elizabeth Dorfman
  */
 struct Tweet {
	 std::string id;           ///< Tweet ID
	 std::string created_at;   ///< Timestamp of when the tweet was created
	 std::string text;         ///< Content of the tweet
	 int like_count;           ///< Number of likes
 };

 /**
  * @brief Entry point of the program.
  *
  * Fetches recent tweets matching a keyword, sorts them by popularity,
  * and writes them to an HTML file in a `<p>`-based layout.
  *
  * @return Exit status code. 0 for success, non-zero for failure.
  * @author Elizabeth Dorfman
  */
 int main() {
	 std::string bearer_token = read_bearer_token();
	 if (bearer_token.empty()) {
		 std::cerr << "Error: BEARER_TOKEN not found in .env file." << std::endl;
		 return 1;
	 }

	 httplib::SSLClient cli("api.twitter.com");
	 httplib::Headers headers = {
		 {"Authorization", "Bearer " + bearer_token},
		 {"User-Agent", "TweetHtmlBuilder/1.0"}
	 };

	 // Define the query and encode it
	 std::string query = "twitter marketing tools -is:retweet";
	 std::string encoded_query = "";
	 for (char c : query) {
		 encoded_query += (c == ' ') ? std::string("%20") : std::string(1, c);
	 }

	 // Create parameter string for the API call
	 std::string params = "query=" + encoded_query +
						  "&tweet.fields=public_metrics,created_at" +
						  "&max_results=2";

	 // Perform GET request to Twitter API
	 auto res = cli.Get(("/2/tweets/search/recent?" + params).c_str(), headers);
	 if (!res || res->status != 200) {
		 std::cerr << "Error: Failed to fetch tweets. Status: " << (res ? res->status : -1) << std::endl;
		 return 1;
	 }

	 // Parse the JSON response
	 nlohmann::json response;
	 try {
		 response = nlohmann::json::parse(res->body);
	 } catch (const std::exception& e) {
		 std::cerr << "Error parsing JSON: " << e.what() << std::endl;
		 return 1;
	 }

	 // Ensure tweet data exists
	 if (!response.contains("data")) {
		 std::cerr << "No tweet data found." << std::endl;
		 return 1;
	 }

	 // Convert JSON tweets to Tweet structs
	 std::vector<Tweet> tweets;
	 for (const auto& tweet : response["data"]) {
		 tweets.push_back({
			 tweet["id"],
			 tweet["created_at"],
			 tweet["text"],
			 tweet["public_metrics"]["like_count"]
		 });
	 }

	 // Sort tweets by like count (descending)
	 std::sort(tweets.begin(), tweets.end(), [](const Tweet& a, const Tweet& b) {
		 return a.like_count > b.like_count;
	 });

	 // Generate HTML file
	 std::ofstream outfile("tweets_output.html");
	 if (!outfile.is_open()) {
		 std::cerr << "Could not open output HTML file." << std::endl;
		 return 1;
	 }

	 outfile << "<!DOCTYPE html>\n<html>\n<head>\n"
			 << "<title>Tweets on Twitter Marketing Tools</title>\n"
			 << "<meta charset=\"UTF-8\">\n"
			 << "<meta name=\"description\" content=\"Tweet export for sentiment and word analysis\">\n"
			 << "<style>body{font-family:Arial, sans-serif; padding:20px;} p{margin-bottom:20px;}</style>\n"
			 << "</head>\n<body>\n"
			 << "<h1>Top Tweets: Twitter Marketing Tools</h1>\n";

	 for (const auto& tweet : tweets) {
		 outfile << "<p>"
				 << tweet.text
				 << " <br><small><em>Likes: " << tweet.like_count
				 << " | Date: " << tweet.created_at << "</em></small></p>\n";
	 }

	 outfile << "</body>\n</html>";
	 outfile.close();

	 std::cout << "HTML page generated with " << tweets.size() << " tweets in tweets_output.html\n";

	 return 0;
 }
