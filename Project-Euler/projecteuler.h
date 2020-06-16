#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <iomanip>

#include "__Chrono.h"
#include "__Primes.h"
#include "__Threads.h"
#include "__Helpers.h"
#include "__stringNum.h"

#ifdef max
  #undef max
#endif

// -----------------------------------------------------------------------------------------------

void func00();

// -----------------------------------------------------------------------------------------------

void func01()
{
	size_t sum = 0;

	for (int i = 0; i < 1000; i++)
	{
		if (!(i % 3) || !(i % 5))
		{
			sum += i;
		}
	}

	std::cout << sum << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func02()
{
	size_t n1 = 1;
	size_t n2 = 2;
	size_t sum(2);

	while (n2 < 4000000)
	{
		std::cout << n2 << std::endl;

		size_t tmp = n2;
		n2 = n2 + n1;
		n1 = tmp;

		if (!(n2 & 1))
		{
			std::cout << " -- even: " << n2 << std::endl;
			sum += n2;
		}
	}

	std::cout << " -- sum = " << sum << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func03()
{
	long long n = 600851475143;
	long long prime = 1;
	long long res = 0;

	size_t cnt = 0;

	while (prime < n)
	{
		cnt++;

		prime = myPrime::getNextPrime(prime);

		if (cnt > 1000)
		{
			std::cout << " -- next prime = " << prime << std::endl;
			cnt = 0;
		}

		if (!(n % prime))
		{
			res = prime;
			n = n / prime;
			std::cout << " -- prime divider = " << res << "; n = " << n << std::endl;
		}
	}

	std::cout << std::endl;
	std::cout << " -- done: " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func04()
{
	size_t max = 0;

	for (size_t i = 100; i < 1000; i++)
	{
		for (size_t j = 100; j < 1000; j++)
		{
			size_t product = i * j;

			if (isPalindromic_base10(product))
			{
				max = product > max ? product : max;
			}
		}
	}

	std::cout << " -- max palindromic = " << max << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func05()
{
	size_t n = 21;

	while (1)
	{
		bool isDivisible = true;

		for (size_t i = 1; i < 21; i++)
		{
			if (n % i)
			{
				isDivisible = false;
				break;
			}
		}

		if (isDivisible)
			break;

		n++;
	}

	std::cout << n << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func06()
{
	const size_t N = 100;

	long long s1 = 0;
	long long s2 = 0;

	for (size_t i = 1; i <= N; i++)
	{
		s1 += i * i;
		s2 += i;
	}

	std::cout << (s2 * s2) - s1 << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func07()
{
	long long n = 1;

	for (int i = 0; i < 10001; i++)
	{
		n = myPrime::getNextPrime(n);
	}

	std::cout << n << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func08()
{
	const char* numbers =
		"73167176531330624919225119674426574742355349194934"
		"96983520312774506326239578318016984801869478851843"
		"85861560789112949495459501737958331952853208805511"
		"12540698747158523863050715693290963295227443043557"
		"66896648950445244523161731856403098711121722383113"
		"62229893423380308135336276614282806444486645238749"
		"30358907296290491560440772390713810515859307960866"
		"70172427121883998797908792274921901699720888093776"
		"65727333001053367881220235421809751254540594752243"
		"52584907711670556013604839586446706324415722155397"
		"53697817977846174064955149290862569321978468622482"
		"83972241375657056057490261407972968652414535100474"
		"82166370484403199890008895243450658541227588666881"
		"16427171479924442928230863465674813919123162824586"
		"17866458359124566529476545682848912883142607690042"
		"24219022671055626321111109370544217506941658960408"
		"07198403850962455444362981230987879927244284909188"
		"84580156166097919133875499200524063689912560717606"
		"05886116467109405077541002256983155200055935729725"
		"71636269561882670428252483600823257530420752963450";

	std::vector<int> vec;

	while (*numbers)
	{
		char ch = *numbers;
		numbers++;
		vec.push_back(ch - 48);
	}

	long long max_product = 0;


	for (size_t i = 0; i < vec.size(); i++)
	{
		size_t max = i + 13;

		if (max < vec.size())
		{
			long long product = 1;

			for (size_t j = i; j < max; j++)
				product *= vec[j];

			if (product > max_product)
				max_product = product;
		}
	}

	std::cout << max_product << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func09()
{
	for (size_t i = 5; i < 1000; i++)
	{
		for (size_t j = i - 1; j > 1; j--)
		{
			for (size_t k = j - 1; k > 0; k--)
			{
				if (k * k + j * j == i * i)
				{
					size_t sum = k + j + i;

					if (sum == 1000)
					{
						sum = k * j * i;
						std::cout << sum << std::endl;
						return;
					}
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------

void func10()
{
	long long n = 2, sum = 0;

	while (1)
	{
		sum += n;
		n = myPrime::getNextPrime(n);

		if (n > 2e6)
			break;
	}

	std::cout << sum << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func11()
{
	const std::string strings[] = {
		"08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08",
		"49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00",
		"81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65",
		"52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91",
		"22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80",
		"24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50",
		"32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70",
		"67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21",
		"24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72",
		"21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95",
		"78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92",
		"16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57",
		"86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58",
		"19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40",
		"04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66",
		"88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69",
		"04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36",
		"20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16",
		"20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54",
		"01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48"
	};

	long long res = 0, prod = 1;
	std::vector< std::vector<int> > vec(20);

	// read and store data
	for (int j = 0; j < 20; j++)
	{
		for (int i = 0; i < 20; i++)
		{
			std::string str = strings[j].substr(3 * i, 2);
			int num = std::stoi(str);
			vec[j].push_back(num);
		}
	}

	// horizontals
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20 - 3; j++)
			{
				prod = 1;
				for (int k = j; k < j + 4; k++)
					prod *= vec[i][k];

				if (prod > res)
					res = prod;
			}
		}
	}

	// verticals
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20 - 3; j++)
			{
				prod = 1;
				for (int k = j; k < j + 4; k++)
					prod *= vec[k][i];

				if (prod > res)
					res = prod;
			}
		}
	}

	// diagonals 1
	{
		for (int i = 0; i < 20 - 3; i++)
		{
			for (int j = 0; j < 20 - 3; j++)
			{
				prod = 1;

				prod *= vec[i + 0][j + 0];
				prod *= vec[i + 1][j + 1];
				prod *= vec[i + 2][j + 2];
				prod *= vec[i + 3][j + 3];

				if (prod > res)
					res = prod;
			}
		}
	}

	// diagonals 2
	{
		for (int i = 0; i < 20 - 3; i++)
		{
			for (int j = 3; j < 20; j++)
			{
				prod = 1;

				prod *= vec[i + 0][j - 0];
				prod *= vec[i + 1][j - 1];
				prod *= vec[i + 2][j - 2];
				prod *= vec[i + 3][j - 3];

				if (prod > res)
					res = prod;
			}
		}
	}

	std::cout << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func12()
{
	size_t answer = 76576500;

	auto getTriangle = [](size_t i) -> size_t
	{
		size_t res = i;

		while (i--)
			res += i;

		return res;
	};

	auto getNumberOfDivisors = [](size_t n)
	{
		size_t cnt = 0;

		for (size_t i = 1; i <= n; i++)
			if (!(n % i))
				cnt++;

		return cnt;
	};

	size_t i = 12000, cnt = 0, n, d;

	while (i++)
	{
		n = getTriangle(i);
		d = getNumberOfDivisors(n);

		if (d > 500)
			break;
		else
			if (cnt == 333)
			{
				std::cout << " i = " << i << "; n = " << n << "; d = " << d << std::endl;
				cnt = 0;
			}

		cnt++;
	}

	std::cout << i << "; val = " << n << std::endl;

	validateResult(answer, n);
}

// -----------------------------------------------------------------------------------------------

void func13()
{
	std::string strings[] = {

		"37107287533902102798797998220837590246510135740250",
		"46376937677490009712648124896970078050417018260538",
		"74324986199524741059474233309513058123726617309629",
		"91942213363574161572522430563301811072406154908250",
		"23067588207539346171171980310421047513778063246676",
		"89261670696623633820136378418383684178734361726757",
		"28112879812849979408065481931592621691275889832738",
		"44274228917432520321923589422876796487670272189318",
		"47451445736001306439091167216856844588711603153276",
		"70386486105843025439939619828917593665686757934951",
		"62176457141856560629502157223196586755079324193331",
		"64906352462741904929101432445813822663347944758178",
		"92575867718337217661963751590579239728245598838407",
		"58203565325359399008402633568948830189458628227828",
		"80181199384826282014278194139940567587151170094390",
		"35398664372827112653829987240784473053190104293586",
		"86515506006295864861532075273371959191420517255829",
		"71693888707715466499115593487603532921714970056938",
		"54370070576826684624621495650076471787294438377604",
		"53282654108756828443191190634694037855217779295145",
		"36123272525000296071075082563815656710885258350721",
		"45876576172410976447339110607218265236877223636045",
		"17423706905851860660448207621209813287860733969412",
		"81142660418086830619328460811191061556940512689692",
		"51934325451728388641918047049293215058642563049483",
		"62467221648435076201727918039944693004732956340691",
		"15732444386908125794514089057706229429197107928209",
		"55037687525678773091862540744969844508330393682126",
		"18336384825330154686196124348767681297534375946515",
		"80386287592878490201521685554828717201219257766954",
		"78182833757993103614740356856449095527097864797581",
		"16726320100436897842553539920931837441497806860984",
		"48403098129077791799088218795327364475675590848030",
		"87086987551392711854517078544161852424320693150332",
		"59959406895756536782107074926966537676326235447210",
		"69793950679652694742597709739166693763042633987085",
		"41052684708299085211399427365734116182760315001271",
		"65378607361501080857009149939512557028198746004375",
		"35829035317434717326932123578154982629742552737307",
		"94953759765105305946966067683156574377167401875275",
		"88902802571733229619176668713819931811048770190271",
		"25267680276078003013678680992525463401061632866526",
		"36270218540497705585629946580636237993140746255962",
		"24074486908231174977792365466257246923322810917141",
		"91430288197103288597806669760892938638285025333403",
		"34413065578016127815921815005561868836468420090470",
		"23053081172816430487623791969842487255036638784583",
		"11487696932154902810424020138335124462181441773470",
		"63783299490636259666498587618221225225512486764533",
		"67720186971698544312419572409913959008952310058822",
		"95548255300263520781532296796249481641953868218774",
		"76085327132285723110424803456124867697064507995236",
		"37774242535411291684276865538926205024910326572967",
		"23701913275725675285653248258265463092207058596522",
		"29798860272258331913126375147341994889534765745501",
		"18495701454879288984856827726077713721403798879715",
		"38298203783031473527721580348144513491373226651381",
		"34829543829199918180278916522431027392251122869539",
		"40957953066405232632538044100059654939159879593635",
		"29746152185502371307642255121183693803580388584903",
		"41698116222072977186158236678424689157993532961922",
		"62467957194401269043877107275048102390895523597457",
		"23189706772547915061505504953922979530901129967519",
		"86188088225875314529584099251203829009407770775672",
		"11306739708304724483816533873502340845647058077308",
		"82959174767140363198008187129011875491310547126581",
		"97623331044818386269515456334926366572897563400500",
		"42846280183517070527831839425882145521227251250327",
		"55121603546981200581762165212827652751691296897789",
		"32238195734329339946437501907836945765883352399886",
		"75506164965184775180738168837861091527357929701337",
		"62177842752192623401942399639168044983993173312731",
		"32924185707147349566916674687634660915035914677504",
		"99518671430235219628894890102423325116913619626622",
		"73267460800591547471830798392868535206946944540724",
		"76841822524674417161514036427982273348055556214818",
		"97142617910342598647204516893989422179826088076852",
		"87783646182799346313767754307809363333018982642090",
		"10848802521674670883215120185883543223812876952786",
		"71329612474782464538636993009049310363619763878039",
		"62184073572399794223406235393808339651327408011116",
		"66627891981488087797941876876144230030984490851411",
		"60661826293682836764744779239180335110989069790714",
		"85786944089552990653640447425576083659976645795096",
		"66024396409905389607120198219976047599490197230297",
		"64913982680032973156037120041377903785566085089252",
		"16730939319872750275468906903707539413042652315011",
		"94809377245048795150954100921645863754710598436791",
		"78639167021187492431995700641917969777599028300699",
		"15368713711936614952811305876380278410754449733078",
		"40789923115535562561142322423255033685442488917353",
		"44889911501440648020369068063960672322193204149535",
		"41503128880339536053299340368006977710650566631954",
		"81234880673210146739058568557934581403627822703280",
		"82616570773948327592232845941706525094512325230608",
		"22918802058777319719839450180888072429661980811197",
		"77158542502016545090413245809786882778948721859617",
		"72107838435069186155435662884062257473692284509516",
		"20849603980134001723930671666823555245252804609722",
		"53503534226472524250874054075591789781264330331690"
	};

	stringNum res("0");

	// 5537376230390876637302048746832985971773659831892672

	for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
		res = res + stringNum(strings[i]);
	
	std::cout << "\n res = " << res.get() << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func14()
{
	size_t max = 0, res = 0;

	for (size_t i = 0; i < 1e6; i++)
	{
		size_t length = getCollatzSequenceLength(i);

		if (length > max)
		{
			max = length;
			res = i;
		}
	}
	
	std::cout << "\n res = " << res << "; length = " << max << std::endl;
}

// -----------------------------------------------------------------------------------------------

struct Node_15 {
	Node_15() : right(nullptr), down(nullptr), val(0) {}
	Node_15 *right, *down;
	int val;
	static long long cnt;

	static void func(Node_15 * n)
	{
		if (n->val)
		{
			if (n->right)
				func(n->right);

			if (n->down)
				func(n->down);
		}
		else
		{
			cnt++;
		}
	}
};

long long Node_15::cnt = 0;

void func15()
{
	const size_t N = 1+20;

	std::vector< std::vector<Node_15> > graph(N);

	// Add values
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Node_15 n;

			if (i < N - 1)
				n.val++;

			if (j < N - 1)
				n.val++;

			graph[i].push_back(n);
		}
	}

	// Add links
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Node_15* n = &graph[i][j];

			if (i < N - 1)
				n->right = &graph[i + 1][j];

			if (j < N - 1)
				n->down = &graph[i][j + 1];
		}
	}

	Node_15::func(&graph[0][0]);

	std::cout << "\n res = " << Node_15::cnt << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func16()
{
	auto mult2 = [](stringNum n)
	{
		return n + n;
	};

	stringNum num("2");
	size_t cnt = 0;

	for (int i = 0; i < 1000-1; i++)
		num = mult2(num);

	std::string* str = &(num.get());

	for (size_t i = 0; i < str->length(); i++)
		cnt += (str->at(i) - 48);

	std::cout << "\n res = " << num.get() << std::endl;
	std::cout << "\n cnt = " << cnt << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func17()
{
	long long num = 0;

	for (size_t i = 1; i <= 1000; i++)
	{
		size_t cnt = 0;
		std::string str = numToStr(i);

		std::cout << i << ": " << str << " == ";

		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] != ' ')
				cnt++;
		}

		std::cout << cnt << std::endl;

		num += cnt;
	}

	std::cout << "\n res = " << num << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func18()
{
	size_t answer = 1074;

	int array[] =
	{
		75,
		95, 64,
		17, 47, 82,
		18, 35, 87, 10,
		20, 04, 82, 47, 65,
		19,  1, 23, 75,  3, 34,
		88,  2, 77, 73,  7, 63, 67,
		99, 65,  4, 28,  6, 16, 70, 92,
		41, 41, 26, 56, 83, 40, 80, 70, 33,
		41, 48, 72, 33, 47, 32, 37, 16, 94, 29,
		53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14,
		70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57,
		91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48,
		63, 66, 04, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31,
		 4, 62, 98, 27, 23,  9, 70, 98, 73, 93, 38, 53, 60,  4, 23
	};

	struct Node {
		int		data;
		size_t	sum;
		Node* left;
		Node* right;
		Node() : data(-1), sum(0), left(nullptr), right(nullptr) 
		{}
	};

	// Build graph
	std::vector< std::vector<Node*> > graph;
	std::vector<Node*> v;
	size_t row(1), max(0);

	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		Node *n = new Node();
		n->data = array[i];
		v.push_back(n);

		if (v.size() == row)
		{
			if (row > 1)
			{
				std::vector<Node*> *prev = &(graph.back());

				for (size_t j = 0; j < prev->size(); j++)
				{
					Node* n = prev->at(j);
					n->left  = v[j+0];
					n->right = v[j+1];

					size_t lSum = n->sum + n->left->data;
					size_t rSum = n->sum + n->right->data;

					n->left ->sum = lSum < n->left ->sum ? n->left ->sum : lSum;
					n->right->sum = rSum < n->right->sum ? n->right->sum : rSum;
				}
			}
			else
			{
				v[0]->sum = v[0]->data;
			}

			graph.push_back(v);
			v.clear();
			row++;
		}
	}


	// Find max value in the last vector
	for (auto* node : graph.back())
		if (node->sum > max)
			max = node->sum;


	// Clear the graph
	for (auto &vec : graph)
		for (auto* node : vec)
			delete node;

	std::cout << "\n res = " << max << std::endl;

	validateResult(answer, max);
}

// -----------------------------------------------------------------------------------------------

void func19()
{
	// 0 = Monday => Sunday = 6
	size_t weekDay(0), cnt(0);

	for (size_t year = 1900; year < 2001; year++)
	{
		bool isLeap = isLeapYear(year);

		for (size_t mon = 0; mon < 12; mon++)
		{
			size_t daysInMonth = getDaysInMonth(mon, isLeap);

			for (size_t day = 0; day < daysInMonth; day++)
			{
				// Count all Sundays that fall on 1st of month between 1901 and 2001
				if (year > 1900 && year < 2001 && day == 0 && weekDay == 6)
					cnt++;

				// Keep track of week days
				weekDay = (++weekDay == 7) ? 0 : weekDay;
			}
		}
	}

	std::cout << "\n res = " << cnt << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func20()
{
	size_t res = 0;

	auto factorial = [](stringNum &num) -> stringNum
	{
		stringNum cnt("1"), one("1"), tmp("1");

		while (cnt.get() != num.get())
		{
			tmp = tmp * cnt;
			cnt = cnt + one;
		}

		return tmp * num;
	};

	stringNum n("100");

	n = factorial(n);

	for (size_t i = 0; i < n.get().length(); i++)
	{
		char ch = n.get()[i];
		size_t num = ch - 48;
		res += num;
	}

	std::cout << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func21()
{
	auto d = [](size_t num) -> size_t
	{
		size_t sum(0);

		for (size_t i = 1; i < num; i++)
			if (num % i == 0)
				sum += i;

		return sum;
	};

	size_t res = 0;

	for (size_t i = 1; i < 10000; i++)
	{
		size_t a = d(i);
		size_t b = d(a);

		if (i != a && i == b)
			res += a;
	}

	std::cout << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

// _p022_names.txt
void func22()
{
	size_t res = 0, cnt = 1;
	std::list<std::string> list;
	std::fstream f;


	f.open("_p022_names.txt", std::fstream::in);


	if (f.is_open())
	{
		std::string line;

		// Read the file and build the graph
		while (std::getline(f, line))
		{
			size_t pos1 = 0, pos2 = 0, p1, p2;
			std::string word;

			while (pos1 < line.length())
			{
				pos2 = line.find_first_of(",", pos1+1);
				word = line.substr(pos1, pos2 - pos1);
				pos1 = pos2;

				p1 = word.find_first_of("\"") + 1;
				p2 = word.find_first_of("\"", p1);

				list.emplace_back(word.substr(p1, p2 - p1));
			}
		}

		f.close();
	}


	list.sort();


	for (auto i : list)
	{
		size_t num = 0;
		std::transform(i.begin(), i.end(), i.begin(), [](unsigned char c) { return std::tolower(c); });

		for (auto ch : i)
			num += (ch - 96);

		res += num * cnt;
		cnt++;
	}

	std::cout << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func23()
{
	size_t res = 0;
	const size_t MAX = 28123;

	auto findNextAbundant = [](size_t num) -> size_t
	{
		size_t res = num + 1;

		while (1)
		{
			size_t sum = 0;
			for (size_t i = 1; i < res; i++)
			{
				if (res % i == 0)
					sum += i;
			}

			if (sum > res)
				break;
			else
				res++;
		}

		return res;
	};

	std::vector<size_t> vecAbundant;

	while (res <= MAX)
	{
		res = findNextAbundant(res);
		vecAbundant.push_back(res);
	}

	// Map of all possible sums of abundant numbers we've found before
	std::map<size_t, bool> mSums;
	for (auto i : vecAbundant)
		for (auto j : vecAbundant)
			mSums[i + j] = true;

	res = 0;
	for (size_t i = 1; i <= MAX; i++)
		if (mSums.count(i) == 0)
			res += i;

	std::cout << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func24()
{
	std::string answer("2783915460"), res;

	std::vector<char> origVec = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::vector<std::string> resVec;

	std::cout << " --- permutations: ";
	getPermutations(resVec, origVec);
	std::cout << "done ---" << std::endl;

	std::cout << " --- sorting: ";
	std::sort(resVec.begin(), resVec.end());
	std::cout << "done ---" << std::endl;

	size_t max = 1000000;

	if( resVec.size() > max )
		res = resVec[max-1];
	else
		for(auto str: resVec)
			std::cout << str << std::endl;

	std::cout << "\n res = " << res << std::endl;

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func25()
{
	size_t targetLength = 1000, i = 0;;

	stringNum a("1");
	stringNum b("1");

	while( ++i )
	{
		if (b.get().length() >= targetLength)
			break;

		getNextFibonacci(a, b);
	}

	std::cout << i + 1 << ": " << b.get() << "; len = " << b.get().length() << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func26()
{
	auto findPattern = [](std::string str) -> size_t
	{
		size_t res = 0, i = 1, len = str.length();

		while (i < str.length())
		{
			std::string sub = str.substr(len - i);

			size_t pos = str.rfind(sub, len-i-1);

			if (pos == std::string::npos || pos + sub.length() > len - i)
			{
				i--;
				break;
			}

			i++;
		}

		return i;
	};

	stringNum one(1);
	stringNum::_precision = 2000;
	size_t max = 0, theNum = 0;

	for (size_t i = 1; i < 1000; i++)
	{
		stringNum num(i);
		stringNum ccc = one / num;

		std::string str = ccc.get();
		str = str.substr(2, str.length() - 3);

		size_t n = findPattern(str);

		if (n > max)
		{
			max = n;
			theNum = i;
		}
	}

	std::cout << "\n res = " << max << "; num = " << theNum << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func27()
{
	long res = 0, max = 0, quadratic = 0, answer = -59231;

	for (long a = -999; a < 1000; a++)
	{
		for (long b = -1000; b <= 1000; b++)
		{
			long n = 0;

			do {

				quadratic = (n * n) + (a * n) + b;
				n++;
			}
			while (myPrime::isPrime(quadratic));

			if (max < n)
			{
				res = a * b;
				max = n;
			}
		}
	}

	std::cout << "\n res = " << res << "; max = " << max << std::endl;

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func28()
{
	auto getNthSum = [](size_t n, size_t &last) -> long long
	{
		size_t sum = 0;

		if (n & 1)
		{
			if (n == 1)
			{
				last = sum = 1;
			}
			else
			{
				for (size_t i = 0; i < 4; i++)
				{
					last += (n - 1);
					sum += last;
				}
			}
		}

		return sum;
	};

	long long sum = 0, answer = 669171001;
	size_t last = 0;

	for (size_t i = 0; i <= 1001; i++)
	{
		sum += getNthSum(i, last);
	}

	std::cout << "\n res = " << sum << std::endl;

	validateResult(answer, sum);
}

// -----------------------------------------------------------------------------------------------

void func29()
{
	size_t MAX = 100u;

	auto pow = [](stringNum num, stringNum pow, stringNum &prev) -> stringNum
	{
		stringNum res(1), one(1), zero;

		if (prev == zero)
		{
			for(stringNum i; i < pow; ++i)
				res = res * num;
		}
		else
		{
			res = prev * num;
		}

		return res;
	};

	std::map<std::string, char> map;

	for (size_t i = 2; i <= MAX; i++)
	{
		stringNum a(i), p;

		for (size_t j = 2; j <= MAX; j++)
		{
			stringNum b(j);

			p = pow(a, b, p);

			std::string* str = &p.get();

			if (!map.count(*str))
				map[*str] = '.';
		}

		std::cout << ".";
	}

	std::cout << "\n res = " << map.size() << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func30()
{
	auto func = [](size_t num) -> size_t
	{
		std::vector<size_t> vec;
		size_t sum = 0, tmp = num;

		while (tmp)
		{
			size_t digit = tmp % 10;
			vec.push_back(digit);
			tmp /= 10;
		}

		for (auto i : vec)
			sum += i*i*i*i*i;

		if (sum == num)
			return sum;

		return 0;
	};


	long long sum = 0;
	for (size_t i = 1; i < 1e6; i++)
	{
		sum += func(i);
	}

	std::cout << "\n res = " << sum - 1 << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func31_helper(size_t &res, size_t MAX, std::vector<size_t> &COEFFC, size_t coeffSize, size_t *VALUES, size_t pos, size_t level = 0)
{
	size_t NUM = MAX / VALUES[pos];

	for (size_t n = 0; n <= NUM; n++)
	{
		COEFFC[pos] = n;

		if (VALUES[pos] == MAX)
		{
			size_t sum = 0;
			for (size_t i = 0; i < coeffSize; i++)
				sum += COEFFC[i] * VALUES[i];

			if (sum == MAX)
				res++;
		}
		else
		{
			func31_helper(res, MAX, COEFFC, coeffSize, VALUES, pos + 1, level + 1);
		}

		if (level == 0)
			std::cout << " -- lev 0 finished: " << n << " of " << NUM << std::endl;
	}

	return;
}

void func31()
{
	size_t VALUES[] = { 1, 2, 5, 10, 20, 50, 100, 200 };

	size_t CNT = sizeof(VALUES) / sizeof(VALUES[0]), res = 0, MAX = VALUES[CNT-1], answer = 73682;
	std::vector<size_t> COEFFC(CNT, 0);

	func31_helper(res, MAX, COEFFC, CNT, VALUES, 0, 0);

	std::cout << "\n res = " << res << std::endl;

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func32()
{
	std::map<size_t, char> map, m;

	auto isPandigital_for3 = [](std::map<size_t, char> m, size_t num1, size_t num2, size_t &num3) -> bool
	{
		m.clear();

		auto isOk = [](std::map<size_t, char> &m, size_t num) -> bool
		{
			while (num)
			{
				size_t digit = num % 10;

				if (digit)
				{
					if (m.count(digit))
						return false;
					else
						m[digit] = '.';

					num /= 10;
				}
				else
				{
					return false;
				}
			}

			return true;
		};

		return isOk(m, num1) && isOk(m, num2) && isOk(m, num3 = num1 * num2) && m.size() == 9u;
	};

	size_t prod = 0, res = 0, max = 2000;

	for (size_t i = 1; i < max; i++)
	{
		for (size_t j = 1; j < max; j++)
		{
			if (isPandigital_for3(m, i, j, prod))
			{
				if (!map.count(prod))
				{
					res += prod;
					map[prod] = '.';
					std::cout << " " << i << " * " << j << " = " << prod << std::endl;
				}
			}
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func33()
{
	auto func = [](size_t &num1, size_t &num2) -> bool
	{
		std::map<size_t, char> m1, m2;
		split(m1, num1);
		split(m2, num2);

		if (m1.count(0) || m2.count(0) || m1.size() < 2u || m2.size() < 2u)
			return false;

		bool doBreak = false;

		for (auto iter1 = m1.begin(); iter1 != m1.end(); ++iter1)
		{
			for (auto iter2 = m2.begin(); iter2 != m2.end(); ++iter2)
			{
				if (iter1->first == iter2->first)
				{
					m1.erase(iter1);
					m2.erase(iter2);
					doBreak = true;
				}

				if (doBreak)
					break;
			}

			if (doBreak)
				break;
		}

		if (m1.size() == 1 && m2.size() == 1)
		{
			num1 = m1.begin()->first;
			num2 = m2.begin()->first;
			return true;
		}

		return false;
	};


	for (size_t i = 10; i < 100; i++)
	{
		for (size_t j = 10; j < 100; j++)
		{
			if (i != j)
			{
				size_t num1 = i;
				size_t num2 = j;

				if (func(num1, num2))
				{
					float fraction1 = float(i) / j;
					float fraction2 = float(num1) / num2;

					if (fraction1 < 1 && fraction1 == fraction2)
					{
						std::cout << i << " ==> " << num1 << std::endl;
						std::cout << j << " ==> " << num2 << std::endl;
						std::cout << std::endl;
					}
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------

void func34()
{
	size_t res = 0;
	std::vector<int> vec;

	for (size_t i = 3; i < 50001; i++)
	{
		split(vec, i);

		size_t sum = 0;

		for (auto num : vec)
			sum += getFactorial(num);

		if (sum == i)
		{
			std::cout << i << std::endl;
			res += sum;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func35()
{
	size_t res = 0, MAX = 1000000, prime = 1, answer = 55;
	std::vector<size_t> vec;

	loopCounter loop(MAX);

	while (prime < MAX)
	{
		bool isOk = true;
		prime = myPrime::getNextPrime(prime);

		getRightDigitRotations(prime, vec);

		for (auto i : vec)
		{
			if (!myPrime::isPrime(i))
			{
				isOk = false;
				break;
			}
		}

		if (isOk)
			res++;

		loop++;
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func36()
{
	size_t res = 0;

	for (size_t i = 1; i < 1e6; i++)
		if (isPalindromic_base10(i) && isPalindromic_base2(i))
			res += i;

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func37()
{
	size_t res = 0, prime = 7, tmp = 0, cnt = 0;

	while (cnt < 11)
	{
		bool isOk = true;

		tmp = prime = myPrime::getNextPrime(prime);

		// Truncate from right
		while (tmp)
		{
			tmp /= 10;
			if (tmp && !myPrime::isPrime(tmp))
			{
				isOk = false;
				break;
			}
		}

		if (isOk)
		{
			size_t dec = 10;

			// Truncate from left
			while (dec < prime)
			{
				size_t n = prime % dec;

				if (!myPrime::isPrime(n))
				{
					isOk = false;
					break;
				}

				dec *= 10;
			}
		}

		if (isOk)
		{
			cnt++;
			std::cout << cnt << ": " << prime << std::endl;
			res += prime;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func38()
{
	// If the number is pandigital [1-9]
	auto isPandigital = [](const std::string &strNum) -> bool
	{
		std::map<char, char> m;

		for (auto ch : strNum)
		{
			if (ch == '0')
				return false;

			if (m.count(ch))
				return false;
			else
				m[ch] = '.';
		}

		return m.size() == 9u;
	};

	stringNum max(0ll);

	size_t num = 0, ii = 0, j = 0;

	for (size_t i = 0; i < 1e6; i++)
	{
		std::string str;

		for (j = 1; j < 10; j++)
		{
			stringNum sNum(i * j);
			if (str.length() + sNum.get().length() > 9)
				break;
			str += sNum.get();
		}

		if (str.length() == 9)
		{
			if (isPandigital(str))
			{
				stringNum tmp(str);
				if (tmp > max)
				{
					num = i;
					ii = j;
					max = tmp;
				}
			}
		}
	}

	std::cout << "\n num = " << num << "; i = 1 .. " << ii << std::endl;
	std::cout << "\n res = " << max.get() << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func39()
{
	auto getNumberOfSolutions = [](const size_t P) -> size_t
	{
		size_t cnt = 0;

		if (P > 2)
		{
			for (size_t a = 1; a < P - 2; a++)
			{
				for (size_t b = 1; b < P - 2; b++)
				{
					if (a + b < P)
					{
						size_t c = P - a - b;

						if (a * a + b * b == c * c)
							cnt++;
					}
				}
			}
		}

		return cnt / 2;
	};

	size_t max = 0, res = 0;

	for (size_t i = 0; i < 1001; i++)
	{
		size_t cnt = getNumberOfSolutions(i);

		if (max < cnt)
		{
			max = cnt;
			res = i;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 ...
void func40()
{
	size_t res = 1, i = 0, dec = 1, len = 0, pos = 0;

	while (pos <= 1e6)
	{
		stringNum num(++i);

		len  = num.get().length();
		pos += len;

		if (pos >= dec)
		{
			char ch = num.get()[len - (pos - dec) - 1];
			std::cout << "digit = " << ch << "; pos = " << dec << std::endl;
			res *= int(ch - 48);
			dec *= 10;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func41()
{
	size_t prime = 987654321, res = 0, cnt = 0, answer = 7652413;

	while (prime--)
	{
		if (isPandigital(prime))
		{
			if (++cnt > 10000)
			{
				std::cout << prime << std::endl;
				cnt = 0;
			}

			if (myPrime::isPrime(prime))
			{
				res = prime;
				break;
			}
		}
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func42()
{
	auto getNthTriangle = [](size_t n) -> size_t
	{
		return n * (n + 1) / 2;
	};

	// [a..z] == [97..122]
	// [A..Z] == [65.. 90]
	auto getWordValue = [](std::string str) -> size_t
	{
		size_t sum = 0;
		for (auto ch : str)
			sum += int(ch) - ((ch < 91) ? 64 : 96);
		return sum;
	};

	size_t res = 0;
	std::list<std::string> list;
	std::fstream f;


	f.open("_p042_words.txt", std::fstream::in);


	if (f.is_open())
	{
		std::string line;

		while (std::getline(f, line))
		{
			size_t pos1 = 0, pos2 = 0, p1, p2;
			std::string word;

			while (pos1 < line.length())
			{
				pos2 = line.find_first_of(",", pos1 + 1);
				word = line.substr(pos1, pos2 - pos1);
				pos1 = pos2;

				p1 = word.find_first_of("\"") + 1;
				p2 = word.find_first_of("\"", p1);

				list.emplace_back(word.substr(p1, p2 - p1));
			}
		}

		f.close();
	}


	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		size_t i = 0, triangle = 0, num = getWordValue(*iter);

		std::cout << " " << *iter << std::endl;

		while (triangle < num)
		{
			triangle = getNthTriangle(++i);

			if (triangle == num)
				res++;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func43()
{
	stringNum answer(16695334890);

	auto isDivisible = [](long long num) -> bool
	{
		stringNum sNum(num);
		size_t prime = 1;

		for (int i = 0; i < 7; i++)
		{
			std::string str = sNum.get().substr(i+1, 3);

			size_t num3 = std::atoi(str.c_str());

			prime = myPrime::getNextPrime(prime);

			if (num3 % prime != 0)
				return false;
		}

		return true;
	};

	int cnt = 0;
	stringNum res(0ll);

	for (long long i = 1234567890; i < 9876543210; i++)
	{
		if (isPandigital(i, 0, 9))
		{
			if (++cnt == 5000)
			{
				std::cout << i << std::endl;
				cnt = 0;
			}

			if (isDivisible(i))
			{
				std::cout << " found: " << i << std::endl;
				res = res + stringNum(i);
			}
		}
	}

	std::cout << "\n res = " << res.get() << std::endl;

	validateResult(answer, res);

	//validate(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func44()
{
	auto getNthPentagonal = [](size_t n) -> size_t
	{
		return n * (3*n - 1) / 2;
	};

	auto isPentagonal = [&](size_t num) -> bool
	{
		size_t pent = 0, i = 100;

		if (num > pent)
		{
			while (pent <= num)
			{
				pent = getNthPentagonal(i);
				i *= 3;
			}

			while (pent >= num)
				pent = getNthPentagonal(i = i / 2);

			while (pent < num)
				pent = getNthPentagonal(++i);
		}

		return num && pent == num;
	};

	size_t MAX = 10000, res = 0;

	for (size_t i = 1; i < MAX; i++)
	{
		size_t ni = getNthPentagonal(i);

		std::cout << i << ": Ni = " << ni << std::endl;

		for (size_t j = i; j < MAX; j++)
		{
			size_t nj = getNthPentagonal(j);

			size_t sum  = nj + ni;
			size_t diff = nj - ni;

			if (isPentagonal(sum) && isPentagonal(diff))
			{
				res = diff;
				std::cout << "\n res = " << res << std::endl;
				return;
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------

void func45()
{
	size_t res = 1, count = 0, cnt = 0;

	auto getNthTriangle = [](long long n) -> long long
	{
		return long long(n * (n + 1) / 2);
	};

	auto getNthPentagonal = [](long long n) -> long long
	{
		return long long(n * (3 * n - 1) / 2);
	};

	auto getNthHexagonal = [](long long n) -> long long
	{
		return long long(n * (2 * n - 1));
	};

	auto isGoodNum = [](long long num, auto func) -> bool
	{
		long long numb = 0;
		size_t i = 100;

		if (num > numb)
		{
			while (numb <= num)
			{
				numb = func(i);
				i *= 3;
			}

			while (numb >= num)
				numb = func(i = i / 2);

			while (numb < num)
				numb = func(++i);
		}

		return num && numb == num;
	};

	while (++res)
	{
		long long num = getNthTriangle(res);

		if (isGoodNum(num, getNthHexagonal))
			if (isGoodNum(num, getNthPentagonal))
				std::cout << "\n " << num << " is a Triangle, Pentagonal and Hexagonal number: " << ++count << "\n" << std::endl;

		if (count == 2)
			break;

		if (++cnt == 5000)
		{
			std::cout << num << std::endl;
			cnt = 0;
		}
	}

	std::cout << "\n res = " << res << std::endl;
}

// -----------------------------------------------------------------------------------------------

void func46()
{
	auto getNextOddComposite = [](size_t num) -> size_t
	{
		num += ((num & 1) ? 2 : 1);

		while (myPrime::isPrime(num))
			num += 2;

		return num;
	};

	auto isOk = [](size_t num) -> bool
	{
		size_t prime = 1;

		while (prime < num)
		{
			prime = myPrime::getNextPrime(prime);
			for(size_t i = 1; i < num; i++)
				if (num == prime + 2 * i * i)
					return true;
		}

		return false;
	};

	size_t composite = 1;

	while (composite < size_t(-1))
	{
		if (!isOk(composite = getNextOddComposite(composite)))
		{
			std::cout << "\n res = " << composite << std::endl;
			break;
		}
	}
}

// -----------------------------------------------------------------------------------------------

void func47()
{
	std::map<size_t, size_t> mPrimes;

	auto isNumberDivisibleByPrimes = [&mPrimes](size_t num, size_t qty) -> bool
	{
		std::map<size_t, bool> map;
		size_t cnt = 0u;

		for (size_t i = 0; i < qty; i++)
		{
			size_t prime = 1, count = 0;

			while (prime <= num)
			{
				if (mPrimes.size() >= ++count)
				{
					prime = mPrimes[count];
				}
				else
				{
					prime = myPrime::getNextPrime(prime);
					mPrimes[count] = prime;
				}

				if (!(num % prime))
				{
					num /= prime;

					if (!map.count(prime))
					{
						map[prime] = true;
						break;
					}
				}
			}
		}

		return num == 1u && map.size() == qty;
	};

	size_t res = 0, qty = 4, cnt = 0, Res = 0, answer = 134043 + 134044 + 134045 + 134046;

	for (int i = 0; i < 1e6; i++)
	{
		if (isNumberDivisibleByPrimes(i, qty))
		{
			cnt++;
			res = i;

			if (cnt == qty)
			{
				std::cout << std::endl;

				for (size_t i = 1; i <= qty; i++)
				{
					Res += res - qty + i;
					std::cout << " -- " << res - qty + i << std::endl;
				}

				break;
			}
		}
		else
		{
			res = 0;
			cnt = 0;
		}

		if (!(i % 10000))
		{
			std::cout << " -- cnt: " << i << std::endl;
		}
	}

	validateResult(Res, answer);
}
// -----------------------------------------------------------------------------------------------

/*
res = 1000368199144695177095375011227646795567793680622934654583760988100234910747716194381428659099527845945869942643191290894720342979906407679647259860434238
4680383260408096910376153703762377136485100631157329514617742467055842668657596018158436664428322845568803131145481515391909753984854966455765134658585827123364
0116622195618817344953167410268890832176466302030669977040862534076609159502279137936809836930637560281385664635877375155877521346022579657984658333400734935862
4342339332981334571237888809283103348760261360175950815609179464026871005243652109980863552142014242903434068560936573231079342194031864413918101238151056509267
3935157603928424725013915940734630015218438110737670217110263075046957334678978218669066484698283466074129673958017977916836098347224322419528453525646818682403
6956956619282555532355807806199752768998384886337478678933158156525205917261433942460098614325923316758337107036262555453185205416611714885822950858158961433759
4463277554380518380921301218836327102231407332201109740102580216469298331766920619646083790732807627360614428085171565006289728508688964226799647192582924058589
5307506745783853655618785595896857562256923489147469228109139156198347541176483580358141286702941585656699420877362863909422415472260150044713306301130720427042
8890504214262819377191859457430220214720118848634591319083375230747696601054742392887106311878302603638131903905200825207205793366671291894623331279369709407422
4187872045970976444309242782187738320257490080824330074991698698239561125811127607863900355221737846690567707344074494145266662103839812840216303448476913957072
3557327166270983722452230467929197472591131574258240648583314154009432782130429546350535740452099845122212642419035501784168245514125486375900077790825392882477
5165356689988274959440589510258798553952770949351004954644542726561747839910718823868177121590423411939224748975107908594805594509880561796372292846955426378221
7625160428008228845552540344494860195267115187092227766195753907211126646150140614744233974765273475619964311852858614167819668340124730487710162006793529985758
8206536772743795633134954545266327187234823394948257598210764016943160434565121179379354564635214630211977266949835589291323575761885949775166307342128638694561
6420552553676731129813718251149464946366307375921921305682356166777609373942574288393071260996216346408803882656913203216069263720618308594298797368458427649178
4843115472077900401692595694119273553511025991265446039366288921743581333200083717105241171504606883543418862024047552177055263424469501298905901938158245938633
6941050248151666798136891566683411977134750943899048871267944689018938504750500112052257424555556257505602132303879103379839503332450206532389891155070138829562
7776388079568721085719649389314265671310596627542214460598805893960060360422692140140209651929425048867029798339635327946045314237554226788198919748178978067895
5093763193658603690898474826976906544473978017455720367929981796023041785852626797271283465789498383642350667978127819110846700

9110846700

 -- Started at   : Tue May 26 21:49:51 2020
 -- Finished at  : Wed May 27 00:49:06 2020
 -- Elapsed time : 10755.1

  OR

 res = 9110846700

 -- Started at   : Wed May 27 09:35:54 2020
 -- Finished at  : Wed May 27 09:35:54 2020
 -- Elapsed time : 0.009
*/
void func48()
{
#if 0

	// Brute force calculation of total number
	// Takes 3 hours
	size_t NUM = 10;
	std::vector<std::shared_ptr<std::thread>> vec;
	std::mutex	mutex;
	stringNum	res;
	size_t numThreads = std::thread::hardware_concurrency(), N = NUM, max(1), min(1), i = 0;

	auto threadFunc = [&](size_t min, size_t max, size_t id)
	{
		stringNum threadRes;

		{
			std::lock_guard<std::mutex> doLock(mutex);
			std::cout << " -- thread[" << (id < 10 ? " " : "") << id << "] started for " << max - min << " (" << min << " - " << max << ")" << std::endl;
		}

		for (size_t i = min; i < max; i++)
		{
			threadRes = threadRes + stringNum_Pow(i + 1, i + 1);

			if (!(i % 10))
			{
				std::lock_guard<std::mutex> doLock(mutex);
				std::cout << " thread " << id << " : " << i << std::endl;
			}
		}

		std::lock_guard<std::mutex> doLock(mutex);
		res = res + threadRes;
		std::cout << " -- thread[" << (id < 10 ? " " : "") << id << "] finished for " << max - min << " (" << min << " - " << max << ")" << std::endl;
	};

	while( min )
	{
		max = N;
		i++;

		if (--numThreads == 0)
		{
			min = 0;
		}
		else
		{
			size_t div = (numThreads + 1) * (numThreads - 1);

			size_t n = N / (div ? div : 2);

			n = n ? n : 1;
			n -= (n > 2) ? (2) : (n > 1 ? 1 : 0);

			N = N > n ? N - n : 0;

			min = N;
		}

		vec.emplace_back(std::make_shared<std::thread>(threadFunc, min, max, i));
	}

	for (size_t i = 0; i < vec.size(); i++)
		vec[i]->join();

	std::cout << "\n res = " << res.get() << std::endl;
	std::cout << std::endl;
	std::cout << "\n" << res.get().substr(res.get().length() - 10) << std::endl;

#else

	// Smart calculation of only last 10 digits
	// Takes milliseconds
	unsigned long long res(0), tmp(0);

	for (size_t i = 0; i < 1000; i++)
	{
		size_t num = i + 1;
		tmp = 1;

		for (size_t j = 0; j < num; j++)
			tmp = tmp * num % 10000000000;

		res += tmp;
	}

	std::cout << "\n res = " << res % 10000000000 << std::endl;

#endif
}

// -----------------------------------------------------------------------------------------------

void func49()
{
	size_t prime1 = 999, prime2 = 0, prime3 = 0, MAX = 10000;

	while (prime1 < MAX)
	{
		prime1 = myPrime::getNextPrime(prime1);

		if (prime1 >= MAX)
			break;

		for (size_t CoNST = 1; CoNST < MAX - prime1; CoNST++)
		{
			prime2 = prime1 + CoNST;

			if (prime2 < MAX && myPrime::isPrime(prime2))
			{
				prime3 = prime2 + CoNST;

				if (prime3 < MAX && myPrime::isPrime(prime3))
				{
					int cnt = 0;
					std::vector<std::string> vec;
					std::vector<char>		 src;

					stringNum num1(prime1), num2(prime2), num3(prime3);

					for (auto ch : num1.get())
						src.push_back(ch);

					getPermutations(vec, src);

					for (auto str : vec)
						if (str == num2.get() || str == num3.get())
							cnt++;

					if (cnt > 1 && CoNST == 3330)
						std::cout << " -- found: " << prime1 << ", " << prime2 << ", " << prime3 << "; CONST = " << CoNST << std::endl;
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------

void func50()
{
	size_t prime = 1, max = 0, res = 0, answer = 997651;
	std::map<size_t, size_t> map;

	while (prime <= 1e6)
	{
		prime = myPrime::getNextPrime(map, prime);

		std::queue<size_t> q;
		size_t cnt = 0, sum = 0;

		for (auto iter : map)
		{
			q.push(iter.second);
			sum += iter.second;
			cnt++;

			if (sum > prime)
			{
				while (sum > prime)
				{
					sum -= q.front();
					cnt--;
					q.pop();
				}
			}

			if (sum == prime)
			{
				if (max < cnt)
				{
					max = cnt;
					res = prime;
					std::cout << " -- found: " << prime << " is a sum of " << cnt << std::endl;
				}

				break;
			}
		}
	}

	validateResult(answer, res);
}

// -----------------------------------------------------------------------------------------------

void func51()
{
	size_t answer = 121313;

	auto getDigitMask = [](size_t num, int digit) -> size_t
	{
		size_t res = 0, cnt = 1;

		while (num)
		{
			if ( num % 10 == digit)
				res += cnt;

			cnt *= 10;
			num /= 10;
		}

		return res;
	};

	bool isFound = false;
	size_t prime = 1, max = 0;

	while (!isFound)
	{
		prime = myPrime::getNextPrime(prime);

		for (int digit = 0; digit < 3; digit++)
		{
			size_t mask = getDigitMask(prime, digit);

			if (mask)
			{
				size_t cnt = 0;

				for (int i = 0; i < 10 - digit; i++)
				{
					size_t newNum = prime + mask * i;

					if (myPrime::isPrime(newNum))
						cnt++;
				}

				if (cnt > max)
				{
					max = cnt;

					std::cout << " new prime = " << prime << std::endl;
					std::cout << " new max   = " << max << "\n" << std::endl;

					if( max == 8)
					{
						isFound = !isFound;
						break;
					}
				}
			}
		}
	}

	validateResult(answer, prime);
}

// -----------------------------------------------------------------------------------------------

void func52()
{
	size_t num = 0, answer = 142857;

	auto hashFunc = [](int digit) -> size_t
	{
		return (11 * digit*digit + 13 * digit + 17);
	};

	auto getHash = [&](size_t num) -> size_t
	{
		size_t sum = 0;

		while (num)
		{
			sum += hashFunc(num % 10);
			num /= 10;
		}

		return sum;
	};

	for (num = 1; num < 1e6; num++)
	{
		bool found = true;
		size_t hash = getHash(num);

		for (int i = 2; i < 7; i++)
		{
			if (getHash(num * i) != hash)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			std::cout << "\n res = " << num << std::endl;
			break;
		}
	}

	validateResult(num, answer);
}

// -----------------------------------------------------------------------------------------------

void func53()
{
	std::map<size_t, stringNum> map;
	size_t num = 0, max = 101, answer = 4075;

	stringNum MAX(1000000);

	for (size_t n = 1; n < max; n++)
	{
		stringNum* N = getFactorial(map, n);

		for (size_t r = 1; r <= n; r++)
		{
			stringNum*  R = getFactorial(map, r);
			stringNum* NR = getFactorial(map, n-r);

			// Don't really need to calculate the exact value : stringNum val = *N / (*R * *NR)
			// This will be enough just to estimate the inequality:
			if ((*N) >= MAX * (*R) * (*NR))
			{
				num++;
			}
		}

		std::cout << " fact[" << n << "] -- done " << std::endl;
	}

	std::cout << "\n res = " << num << std::endl;

	validateResult(num, answer);
}

// -----------------------------------------------------------------------------------------------

void func54()
{
	typedef std::vector<std::string> vecStr;

	enum cards { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
	enum suits { HEARTS = 1, SPADES, DIAMONDS, CLUBS };
	enum hands { HIGH_CARD, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH };

	struct Card {
		cards value;
		suits suit;

		bool operator > (const Card& other) { return value >  other.value; }
		bool operator >=(const Card& other) { return value >= other.value; }
		bool operator < (const Card& other) { return value <  other.value; }
		bool operator ==(const Card& other) { return value == other.value; }
		bool operator !=(const Card& other) { return value != other.value; }
	};

	// ===========================================================================================

	auto readFile = [](auto &list) -> void
	{
		std::fstream f;
		f.open("_p054_poker.txt", std::fstream::in);

		if (f.is_open())
		{
			std::string line;
			vecStr		vv[3];

			while (std::getline(f, line))
			{
				split(line, vv[0]);

				vv[1].clear();
				vv[2].clear();

				for (size_t i = 0; i < 10; i++)
				{
					(i < 5 ? vv[1] : vv[2]).emplace_back(vv[0][i]);
				}

				list.emplace_back(std::make_pair(vv[1], vv[2]));
			}

			f.close();
		}
	};

	auto getHands = [](std::pair<vecStr, vecStr>& p, Card *h1, Card *h2)
	{
		for (int i = 0; i < 2; i++)
		{
			Card   * hand = i ? h2: h1;
			vecStr * vec  = i ? &p.second : &p.first;

			for (size_t i = 0; i < vec->size(); i++)
			{
				Card* card = &hand[i];

				char value = (*vec)[i][0];
				char suit  = (*vec)[i][1];

				switch (suit)
				{
					case 'H':
						card->suit = suits::HEARTS;
						break;

					case 'S':
						card->suit = suits::SPADES;
						break;

					case 'D':
						card->suit = suits::DIAMONDS;
						break;

					case 'C':
						card->suit = suits::CLUBS;
						break;
				}

				if (value < 58)
				{
					card->value = cards(value - 48);
				}
				else
				{
					switch (value)
					{
						case 'T':
							card->value = cards::TEN;
							break;

						case 'J':
							card->value = cards::JACK;
							break;

						case 'Q':
							card->value = cards::QUEEN;
							break;

						case 'K':
							card->value = cards::KING;
							break;

						case 'A':
							card->value = cards::ACE;
							break;
					}
				}
			}
		}
	};

	auto getMaxQty = [](std::map<cards, size_t> &map, size_t max = 0) -> size_t
	{
		for (auto iter : map)
			if (iter.second > max)
				max = iter.second;
		return max;
	};

	auto getCombination = [&](hands &combination, Card hand[5], std::map<cards, size_t> &m_cards, std::map<suits, size_t> &m_suits)
	{
		size_t total = 0, cnt = 0;

		for (size_t i = 0; i < 5; i++)
		{
			Card* card = &hand[i];
			total += card->value;
			m_cards[card->value] = m_cards.count(card->value) ? m_cards[card->value] + 1 : 1;
			m_suits[card->suit ] = m_suits.count(card->suit ) ? m_suits[card->suit ] + 1 : 1;
		}

		// decide what is the current combination on the hand
		switch (m_cards.size())
		{
			case 2:																				// FOUR_OF_A_KIND			-- Four cards of the same value
				combination = (getMaxQty(m_cards) == 4) ? FOUR_OF_A_KIND : FULL_HOUSE;			// FULL_HOUSE				-- Three of a kind and a pair
				break;

			case 3:																				// THREE_OF_A_KIND			-- Three cards of the same value
				combination = (getMaxQty(m_cards) == 3) ? THREE_OF_A_KIND : TWO_PAIRS;			// TWO_PAIRS				-- Two different pairs
				break;

			case 4:																				// ONE_PAIR					--  Two cards of the same value
				combination = ONE_PAIR;
				break;

			case 5:
				switch (m_suits.size())
				{
					case 1:
						if (hand[4].value - hand[0].value == 4u)
							combination = (hand[4].value == cards::ACE)							// ROYAL_FLUSH				-- Ten, Jack, Queen, King, Ace, in same suit
								? ROYAL_FLUSH : STRAIGHT_FLUSH;									// STRAIGHT_FLUSH			-- All cards are consecutive values of same suit
						else
							combination = FLUSH;												// FLUSH					-- All cards of the same suit
						break;

					default:
						if (hand[4].value - hand[0].value == 4u)
							combination = STRAIGHT;												// STRAIGHT					-- All cards are consecutive values
						else
							combination = HIGH_CARD;											// HIGH_CARD				-- Highest value card
						break;
				}
				break;
		}
	};

	// 
	auto findInMap = [](std::map<cards, size_t> &m_cards, size_t val, bool isMax) -> size_t
	{
		size_t cnt = 0, arr[2] = {0, 0};

		for (auto iter : m_cards)
			if (iter.second == val)
				arr[cnt++] = iter.first;

		return isMax ? (arr[0] > arr[1] ? arr[0] : arr[1]) : (arr[0] > arr[1] ? arr[1] : arr[0]);
	};

	auto getDrawResult = [&](const hands combination, Card p1Hand[5], Card p2Hand[5], std::map<cards, size_t> &m_cards1, std::map<cards, size_t> &m_cards2) -> size_t
	{
		switch (combination)
		{
			case ONE_PAIR: {

					size_t val1 = findInMap(m_cards1, 2, true);
					size_t val2 = findInMap(m_cards2, 2, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					for (size_t i = 0; i < 5; i++)
						if (p1Hand[4 - i] != p2Hand[4 - i])
							return p1Hand[4 - i] > p2Hand[4 - i] ? 1 : 2;
				}
				break;

			case TWO_PAIRS: {

					size_t val1 = findInMap(m_cards1, 2, true);
					size_t val2 = findInMap(m_cards2, 2, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 2, false);
					val2 = findInMap(m_cards2, 2, false);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 1, true);
					val2 = findInMap(m_cards2, 1, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;
				}
				break;

			case THREE_OF_A_KIND: {
			
					size_t val1 = findInMap(m_cards1, 3, true);
					size_t val2 = findInMap(m_cards2, 3, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 1, true);
					val2 = findInMap(m_cards2, 1, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 1, false);
					val2 = findInMap(m_cards2, 1, false);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;
				}
				break;

			case FULL_HOUSE: {

					size_t val1 = findInMap(m_cards1, 3, true);
					size_t val2 = findInMap(m_cards2, 3, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 2, true);
					val2 = findInMap(m_cards2, 2, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;
				}
				break;

			case FOUR_OF_A_KIND: {
			
					size_t val1 = findInMap(m_cards1, 4, true);
					size_t val2 = findInMap(m_cards2, 4, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;

					val1 = findInMap(m_cards1, 1, true);
					val2 = findInMap(m_cards2, 1, true);

					if (val1 != val2)
						return val1 > val2 ? 1 : 2;
				}
				break;

			case STRAIGHT:
			case STRAIGHT_FLUSH:
				if (p1Hand[0] != p2Hand[0])
					return p1Hand[0] > p2Hand[0] ? 1 : 2;
				break;

			case FLUSH:
			case HIGH_CARD:
				for (size_t i = 0; i < 5; i++)
					if (p1Hand[4 - i] != p2Hand[4 - i])
						return p1Hand[4 - i] > p2Hand[4 - i] ? 1 : 2;
				break;

			case ROYAL_FLUSH:
				break;
		}

		return 0;
	};

	// 0 == draw
	// 1 = Player1 wins
	// 2 = Player2 wins
	auto getWinner = [&](std::pair<vecStr, vecStr> &pair) -> size_t
	{
		hands p1Combination, p2Combination;

		Card p1Hand[5];
		Card p2Hand[5];

		std::map<cards, size_t> m_cards1, m_cards2;
		std::map<suits, size_t> m_suits1, m_suits2;

		getHands(pair, p1Hand, p2Hand);

		std::sort(std::begin(p1Hand), std::end(p1Hand));
		std::sort(std::begin(p2Hand), std::end(p2Hand));

		getCombination(p1Combination, p1Hand, m_cards1, m_suits1);
		getCombination(p2Combination, p2Hand, m_cards2, m_suits2);

		if (p1Combination != p2Combination)
		{
			return (p1Combination > p2Combination) ? 1 : 2;
		}

		return getDrawResult(p1Combination, p1Hand, p2Hand, m_cards1, m_cards2);
	};

	// ===========================================================================================

	size_t res = 0, i = 0, answer = 376;
	std::list<std::pair<vecStr, vecStr>> list;


	readFile(list);


	for (auto pair : list)
	{
		std::cout << std::setw(5) << std::right << ++i << ": [ ";

		for (auto str : pair.first)
			std::cout << str << " ";

		std::cout << "] vs [ ";

		for (auto str : pair.second)
			std::cout << str << " ";

		size_t outcome = getWinner(pair);

		if (outcome == 1)
			res++;

		std::cout << "] : winner is " << (outcome == 1 ? "Player 1" : outcome == 2? "Player 2" : "None") << std::endl;
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func55()
{
	size_t res = 0, i = 0, answer = 249;

	auto iterateToPalindrome = [](size_t num) -> size_t
	{
		size_t cnt = 0;
		stringNum n(num);

		do {

			std::string str;

			for (auto iter = n.get().rbegin(); iter != n.get().rend(); ++iter)
				str.push_back(*iter);

			n = n + stringNum(str);

			if (++cnt > 50)
				return 0;
		}
		while( !isPalindromic(n.get()));

		return cnt;
	};


	for (size_t i = 1; i < 10000; i++)
	{
		size_t cnt = iterateToPalindrome(i);

		if (!cnt)
			res++;
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func56()
{
	size_t res = 0, i = 0, answer = 972;

	auto getDigitalSum = [](stringNum &n) -> size_t
	{
		size_t res = 0;

		for (auto ch : n.get())
			res += (ch - 48);

		return res;
	};

	for (size_t i = 1; i < 101; i++)
	{
		for (size_t j = 1; j < 101; j++)
		{
			stringNum n = stringNum_Pow(i, j);

			size_t sum = getDigitalSum(n);

			if (sum > res)
				res = sum;
		}

		std::cout << " i = " << i << std::endl;
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func57()
{
	size_t res = 0, i = 0, answer = 153;

	stringNum numerator = 1, denominator = 2, n = 2, d = 1, tmp;

	while (++i < 1001)
	{
		tmp = d;
		d	= n;
		n	= tmp;

		n = d + d + n;

		numerator = n + d;
		denominator = n;

		std::cout << numerator.get() << " / " << denominator.get() << std::endl;

		if (numerator.get().length() > denominator.get().length())
			res++;
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func58()
{
	size_t res = 0, dist = 1, curr = 1, answer = 26241;

	auto getPercentage = [&](std::vector<size_t> &vec, myPrime::container &map) -> size_t
	{
		size_t cnt = 0, max = *map.rbegin();

		for (auto num : vec)
		{
			if (num <= max)
			{
				if (map.count(num))
					cnt++;
			}
			else
			{
				if (myPrime::isPrime(num))
				{
					map.insert(num);
					cnt++;
				}
			}
		}

		return size_t(100.0 * cnt / vec.size());
	};

	myPrime::container map;
	std::vector<size_t> vec;
	vec.push_back(curr);

	std::cout << " Populating map with primes ... ";
	myPrime pr;
	pr.getPrimes(map, 1, 688590100);
	std::cout << "done!\n" << std::endl;

	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			curr = curr + dist + 1;
			vec.push_back(curr);
		}

		dist += 2;

		size_t percent = getPercentage(vec, map);

		if (percent < 10u)
		{
			std::cout << " curr = " << curr << "; percent = " << percent << std::endl;
			res = dist;
			break;
		}
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func59()
{
	size_t res = 0, answer = 129448;
	char pswd[4] = { 0 };

	auto attackAgainstPswd = [&pswd](const std::string str) -> std::string
	{
		std::string res;

		for (size_t i = 0; i < str.length(); i++)
		{
			size_t pos = i % 3;
			char ch = str[i] ^ pswd[pos];
			res.push_back(ch);
		}

		return res;
	};

	auto readFile = []() -> std::string
	{
		std::string res;
		std::fstream f;
		f.open("_p059_cipher.txt", std::fstream::in);

		if (f.is_open())
		{
			std::string line;

			while (std::getline(f, line))
			{
				std::vector<std::string> vec;
				split(line, vec, ',');

				for (auto s : vec)
					res.push_back(std::atoi(s.c_str()));
			}

			f.close();
		}

		return res;
	};

	auto decipher = [&]() -> std::string
	{
		const char* words[] = { "the", " a ", "number" };
		size_t SIZE = sizeof(words) / sizeof(words[0]);

		std::string str = readFile();

		// 3 x [a .. z]
		for (char ch1 = 97; ch1 < 123; ch1++)
		{
			pswd[0] = ch1;

			for (char ch2 = 97; ch2 < 123; ch2++)
			{
				pswd[1] = ch2;

				for (char ch3 = 97; ch3 < 123; ch3++)
				{
					pswd[2] = ch3;

					std::string strRes = attackAgainstPswd(str);

					size_t cnt = 0;
					for (size_t i = 0; i < SIZE; i++)
						if (strRes.find(words[i]) != std::string::npos)
							cnt++;

					if (cnt == SIZE)
						return strRes;
				}
			}
		}

		return "";
	};

	// ----------------------------------------------------------------------------------------------

	std::string strRes = decipher();

	doPrint(strRes);
	std::cout << " pswd = '" << pswd << "'" << std::endl;

	for (auto ch : strRes)
		res += size_t(ch);

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void func60()
{
	struct cmp { bool operator() (const size_t* a, const size_t* b) const { return *a < *b; } };
	typedef std::set<const size_t*, cmp> customSet;

	myPrime pr;
	myPrime::container map;
	size_t res = 0, answer = 26033;
	std::vector<customSet> vec;

	auto isPrime = [](myPrime::container &map, size_t n) -> bool
	{
		if (map.count(n))
			return true;

		return myPrime::isPrime(n);
	};

	auto concatenate_and_Prime_Check = [&isPrime, &map](size_t n1, size_t n2) -> bool
	{
		size_t dec1(n2), dec2(n1), N1(1), N2(1);

		do N1 *= 10; while (dec1 /= 10);
		do N2 *= 10; while (dec2 /= 10);

		return isPrime(map, n1*N1 + n2) && isPrime(map, n1 + n2*N2);
	};

	auto getSumOfTheSet = [](customSet &set) -> size_t
	{
		size_t sum = 0;
		for (auto num : set)
			sum += *num;
		return sum;
	};

	auto printSet = [&res](customSet *set)
	{
		std::cout << std::endl;

		if (set)
		{
			std::map<size_t, bool> m;
			size_t sum = 0;

			for (auto num : *set)
				m[*num] = true;

			for (auto num : m)
			{
				sum += num.first;
				std::cout << num.first << " ";
			}

			std::cout << "\n -- sum = " << sum << std::endl;
			res = sum;
		}
		else
		{
			std::cout << " -- none found " << std::endl;
		}

		std::cout << std::endl;
	};

	auto doit = [&](size_t qty) -> customSet*
	{
		size_t min = size_t(-1);
		customSet* ptr = nullptr;

		for (auto set = vec.begin(); set != vec.end(); ++set)
		{
			if (set->size() == qty)
			{
				// Max element will be the rightmost:
				const size_t* max = *set->rbegin();

				for (auto prime = map.begin(); prime != map.end(); ++prime)
				{
					if (*prime > * max)
					{
						bool prime_isOk = true;

						for (auto num : *set)
						{
							if (!concatenate_and_Prime_Check(*num, *prime))
							{
								prime_isOk = false;
								break;
							}
						}

						if (prime_isOk)
						{
							set->insert(&*prime);

							size_t sum = getSumOfTheSet(*set);

							if (sum < min)
							{
								min = sum;
								ptr = &*set;
							}

							break;
						}
					}
				}
			}
		}

		return ptr;
	};

	// -------------------------------------------------------------------------------------------

	std::cout << " -- Building map of primes ...";
	pr.getPrimes(map, 1, 11111);
	std::cout << " done!" << std::endl;


	std::cout << " -- Building initial vector of sets of 2 ...";
	for (auto prime1 = map.begin(); prime1 != map.end(); ++prime1)
	{
		for (auto prime2 = map.begin(); prime2 != map.end(); ++prime2)
		{
			if (*prime1 < *prime2 && concatenate_and_Prime_Check(*prime1, *prime2))
			{
				customSet s;
				vec.emplace_back(s);
				vec.back().insert(&*prime1);
				vec.back().insert(&*prime2);
			}
		}
	}
	std::cout << " done!" << std::endl;


	for (size_t i = 2; i < 5; i++)
	{
		std::cout << "\n -- Building sets of " << i+1 << " ...";

		auto ptr = doit(i);
		printSet(ptr);
	}

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

class func61_helper {

	public:

		// https://isocpp.org/wiki/faq/pointers-to-members
		typedef size_t(func61_helper::*polygonalFunc)(size_t);

		func61_helper()
		{
			polygonalFunc funcs[] = { &func61_helper::getTriangle, &func61_helper::getSquare, &func61_helper::getPentagonal, &func61_helper::getHexagonal, &func61_helper::getHeptagonal, &func61_helper::getOctagonal };

			for (size_t i = 0; i < sizeof(funcs) / sizeof(funcs[0]); i++)
			{
				std::vector<size_t> v;
				vec.push_back(v);

				fillVector(vec, i, funcs[i]);
			}
		}

		void find_thread(size_t& result)
		{
			const size_t numThreads = 10;
			std::vector<std::shared_ptr<std::thread>> vecThreads;
			threadsStop = false;

			for (size_t id = 0; id < numThreads; id++)
				vecThreads.emplace_back(
					std::make_shared<std::thread>
						(&func61_helper::thFunc, this, id, numThreads, std::ref(result)));

			for (auto th : vecThreads)
				th->join();

			return;
		}

	private:

		std::vector<std::vector<size_t>> vec;
		std::mutex	mutex, mtx;
		bool threadsStop;

		size_t getTriangle   (size_t n) { return n * (1*n + 1) / 2;	};
		size_t getSquare     (size_t n) { return n * (1*n + 0) / 1;	};
		size_t getPentagonal (size_t n) { return n * (3*n - 1) / 2;	};
		size_t getHexagonal  (size_t n) { return n * (2*n - 1) / 1;	};
		size_t getHeptagonal (size_t n) { return n * (5*n - 3) / 2;	};
		size_t getOctagonal  (size_t n) { return n * (3*n - 2) / 1;	};

		void thFunc(size_t id, size_t totalThreads, size_t &result)
		{
			std::vector<size_t> vec_numbers(6);
			size_t res = recursiveSearch_thread(vec, vec_numbers, 0, 6, id, totalThreads);

			if (res > 0)
				result = res;
		}

		size_t recursiveSearch_thread(const std::vector<std::vector<size_t>>& vec, std::vector<size_t>& vec_data, size_t vecNum, size_t MAX_VEC_NUM, size_t id, size_t numThreads)
		{
			{
				std::lock_guard<std::mutex> doLock(mtx);
					if (threadsStop)
						return 1;
			}

			size_t initNum = vec[vecNum][0];

			size_t min = vecNum ? 1 : id + 80;
			size_t add = vecNum ? 1 : numThreads;

			for (size_t i = min; i < vec[vecNum].size(); i += add)
			{
				vec_data[vecNum] = initNum + i - 1;

				if (checkNumbers(vec_data, vecNum))
				{
					if (vecNum == 3 || vecNum == 4)
					{
						std::vector<size_t> v;

						for (size_t i = 0; i < vec_data.size(); i++)
						{
							size_t pos = vec_data[i] - vec[i][0] + 1;
							v.emplace_back(vec[i][pos]);
						}

						size_t num = getNumOfPairs(v);

						if ((vecNum == 3 && num < 2) || (vecNum == 4 && num < 4))
							continue;
					}

					// at the deepest level of recursion
					if (vecNum == MAX_VEC_NUM - 1)
					{
						std::vector<size_t> v;
						std::vector<std::vector<size_t>> vec_permutations;

						for (size_t i = 0; i < vec_data.size(); i++)
						{
							size_t pos = vec_data[i] - vec[i][0] + 1;
							v.emplace_back(vec[i][pos]);
						}

						getPermutations(vec_permutations, v);

						for (auto v : vec_permutations)
						{
							if (isCyclic(v))
							{
								size_t res = 0;

								for (auto n : v)
									res += n;

								{
									std::lock_guard<std::mutex> doLockBool(mtx);
										threadsStop = true;
								}

								std::lock_guard<std::mutex> doLock(mutex);

									std::cout << std::endl;
									std::cout << " ------------------------------------------------------------ " << std::endl;
									for (auto n : v)
										std::cout << " " << n;
									std::cout << std::endl;
									std::cout << " ------------------------------------------------------------ " << std::endl;
									std::cout << std::endl;

									return res;
							}
						}
					}
					else
					{
						size_t res = recursiveSearch_thread(vec, vec_data, vecNum + 1, MAX_VEC_NUM, id, numThreads);

						if (vecNum < 2)
						{
							std::lock_guard<std::mutex> doLock(mutex);
							
								for (size_t i = 0; i < vecNum; i++)
									std::cout << "\t";

								std::cout << " thread " << id << ": level " << vecNum << " : " << i << " out of " << vec[vecNum].size() << std::endl;
						}

						if (res > 1)
							return res;
					}
				}
			}

			return 0;
		}

		// Get first or last 2 digits of a 4-digit number
		size_t get2digits(size_t n, bool first)
		{
			size_t res = 0;

			n = first ? n / 100 : n;

			for (int i = 0; i < 2; i++)
			{
				int digit = n % 10;
				n /= 10;
				digit *= i ? 10 : 1;
				res += digit;
			}

			return res;
		};

		// Checks if the vector is cyclic ( i.e., { 1234, 3456, 5612 } )
		bool isCyclic(const std::vector<size_t>& vec)
		{
			bool b_first(true), b_last(false);

			for (size_t i = 0; i < vec.size() - 1; i++)
				if (get2digits(vec[i], b_last) != get2digits(vec[i + 1], b_first))
					return false;

			return get2digits(vec[0], b_first) == get2digits(vec.back(), b_last);
		};

		void fillVector(std::vector<std::vector<size_t>>& vec, size_t n, polygonalFunc func)
		{
			size_t i = 0, num = 0;

			do {

				num = ((*this).*(func))(++i);
				if (num > 1000)
				{
					if (!vec[n].size())
						vec[n].push_back(i);

					vec[n].push_back(num);
				}

			} while (num < 10000);
		};

		bool checkNumbers(std::vector<size_t> &vec, size_t N)
		{
			if (N)
			{
				std::set<size_t> s;

				for (size_t i = 0; i <= N; i++)
					s.insert(vec[i]);

				return s.size() == N+1;
			}

			return true;
		}

		int getNumOfPairs(std::vector<size_t> &vec)
		{
			int max = 0;
			bool b_first(true), b_last(false);
			std::vector<std::vector<size_t>> PERM;

			getPermutations(PERM, vec);

			for (auto v : PERM)
			{
				int cnt = 0;

				for (size_t i = 0; i < v.size() - 1; i++)
					if (get2digits(v[i], b_last) == get2digits(v[i + 1], b_first))
						cnt++;

				if (get2digits(v[0], b_first) == get2digits(v.back(), b_last))
					cnt++;

				if (cnt > max)
					max = cnt;
			}

			return max;
		}
};

void func61()
{
	size_t res = 0, answer = 28684;

	func61_helper hlp;

	// This worked, but took over 2 hours to find the result
	hlp.find_thread(res);

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

// Get all permutations of a number that are cubes:
void func62_permutations(std::vector<std::string> &resVec, const std::vector<char> &srcVec, size_t level = 0, char prev = '.')
{
	auto isCube = [](const size_t num) -> size_t
	{
		size_t root3 = size_t(pow(num, 1.0 / 3));

		for (int i = 0; i < 2; i++)
			if (num == (root3 + i) * (root3 + i) * (root3 + i))
				return root3 + i;

		return 0;
	};

	if (srcVec.size() == 2)
	{
		std::string str_tmp;

		resVec.emplace_back(str_tmp);
		resVec.back().push_back(srcVec[0]);
		resVec.back().push_back(srcVec[1]);

		resVec.emplace_back(str_tmp);
		resVec.back().push_back(srcVec[1]);
		resVec.back().push_back(srcVec[0]);
	}
	else
	{
		std::vector<char>			vec_char;
		std::vector<std::string>	vec_str;

		for (size_t i = 0; i < srcVec.size(); i++)
		{
			char ch = srcVec[i];

			// Skip all permutations that are not cubes: check the first two digits (the number must be reversed later)
			// https://ru.wikipedia.org/wiki/Куб_(алгебра)
			if (level == 1)
			{
				bool doSkip = false;

				switch (prev)
				{
					case '0':
						doSkip = (ch != '0');
						break;

					case '5':
						doSkip = (ch != '2' && ch != '7');
						break;

					case '4':
					case '8':
						doSkip = (int(ch - 48) & 1);			// must be even
						break;

					case '2':
					case '6':
						doSkip = !(int(ch - 48) & 1);			// must be odd
						break;
				}

				if (doSkip)
					continue;
			}

			for (size_t j = 0; j < srcVec.size(); j++)
				if (j != i)
					vec_char.emplace_back(srcVec[j]);

			// -= Recursive call =-
			func62_permutations(vec_str, vec_char, level+1, ch);

			for (auto str = vec_str.begin(); str != vec_str.end(); ++str)
			{
				if (level == 0)
				{
					if (str->back() != '0')
					{
						std::string s = srcVec[i] + *str;

						reverseStr(s);

						size_t num = std::atoll(s.c_str());

						if (isCube(num))
							resVec.emplace_back(s);
					}
				}
				else
				{
					resVec.emplace_back(srcVec[i] + *str);
				}
			}

			vec_char.clear();
			vec_str.clear();
		}
	}

	return;
}

void func62()
{
	size_t res = 0, answer = 127035954683;

// #define STUPID

#ifdef STUPID

	myThreadLoop th(8);
	std::mutex& m = th.getMutex(myThreadLoop::MUTEX_CONSOLE);

	struct sharedData {
		size_t res;
	} sd;

	struct threadData {
		size_t id;
		std::vector<int>			vec_pow3;
		std::vector<std::string>	vec_perm;
	} td;

	// -------------------------------------------------------------------------------------------

	auto isCube = [](const size_t num) -> size_t
	{
		size_t root3 = size_t(pow(num, 1.0 / 3));

		for (int i = 0; i < 2; i++)
			if (num == (root3 + i) * (root3 + i) * (root3 + i))
				return root3 + i;

		return 0;
	};

	auto mainFunc = [&](sharedData& sd, void* threadDataPtr, size_t i)
	{
		threadData* td = static_cast<threadData*>(threadDataPtr);
		std::vector<char> vec_chars;
		size_t cnt = 0, res = i * i * i;

		// Get all possible permutations:
		split(td->vec_pow3, res);
		for (auto n : td->vec_pow3)
			vec_chars.push_back(n + 48);

		td->vec_perm.clear();
		func62_permutations(td->vec_perm, vec_chars);

		// Use set, as some numbers will duplicate (there are duplicate digits in some cubes)
		std::set<size_t> s;
		for (auto str : td->vec_perm)
		{
			size_t num = std::atoll(str.c_str());
			s.insert(num);
		}

		cnt = s.size();

		{
			std::lock_guard<std::mutex> doLock(m);
				std::cout << " -- th[" << ((td->id < 10) ? " " : "") << td->id << "] says: " << i << " ^ 3 = " << res << "; cnt = " << cnt << std::endl;
		}

		if (cnt == 5)
		{
			std::lock_guard<std::mutex> doLockData(th.getMutex(myThreadLoop::MUTEX_DATA));
				th.doStop();
				sd.res = res;

			std::lock_guard<std::mutex> doLockConsole(th.getMutex(myThreadLoop::MUTEX_CONSOLE));
				std::cout << std::endl;
				std::cout << " -- th[" << ((td->id < 10) ? " " : "") << td->id << "] says: -- found : " << i << " ^ 3 = " << res << std::endl;

				for (auto num : s)
					std::wcout << num << std::endl;

				std::cout << std::endl;
		}
	};

	// -------------------------------------------------------------------------------------------

	sd.res = 0;
	td.id  = 0;

	th.exec(mainFunc, 5025, size_t(-1), sd, td);

	res = sd.res;

#else

	typedef std::map<int, int> smallMap;

	auto getMap = [](const size_t num, smallMap &map) -> void
	{
		map.clear();

		size_t cube = num * num * num;

		while (cube)
		{
			int digit = cube % 10;
			cube /= 10;
			map[digit] = map.count(digit) ? map[digit] + 1 : 1;
		}

		return;
	};

	// ----------------------------------------------------------------------------------------------

	const size_t NUM = 5;

	smallMap mCube;
	std::multimap<smallMap, size_t> MAP;

	for (size_t i = 1; i < size_t(-1); i++)
	{
		getMap(i, mCube);

		MAP.insert(std::make_pair(mCube, i));

		if (MAP.count(mCube) == NUM)
		{
			auto range = MAP.equal_range(mCube);

			res = range.first->second * range.first->second * range.first->second;

			for (auto i = range.first; i != range.second; ++i)
				std::cout << i->second << std::endl;

			MAP.clear();
			break;
		}
	}

#endif

#undef STUPID

	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}

// -----------------------------------------------------------------------------------------------

void aaa(size_t i, size_t a)
{
	std::cout << std::endl;
	std::cout << " thread says : " << a << std::endl;
	std::cout << std::endl;
}

void func63()
{
	size_t res = 0, answer = 127035954683;


/*
	myThreadLoop_2 th(1);

	th.exec(aaa, 12345, 999);
*/


	std::cout << "\n res = " << res << std::endl;

	validateResult(res, answer);
}
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

void func00()
{
	func63();
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

void func67()
{
	size_t answer = 7273;

	auto parseStrToVec = [](std::vector<size_t> &vec, std::string str)
	{
		vec.clear();
		std::string tmp;
		size_t pos1(0), pos2 = str.find_first_of(" ", pos1);

		while (pos2 != std::string::npos)
		{
			tmp = str.substr(pos1, pos2 - pos1);
			vec.push_back(std::atoi(tmp.c_str()));
			pos1 = pos2;
			pos2 = str.find_first_of(" ", pos1 + 1);
		}

		tmp = str.substr(pos1, pos2 - pos1);
		vec.push_back(std::atoi(tmp.c_str()));
	};

	struct Node {
		int		data;
		size_t	sum;
		Node* left;
		Node* right;
		Node() : data(-1), sum(0), left(nullptr), right(nullptr)
		{}
	};

	// Build graph
	std::fstream f;
	std::vector< std::vector<Node*> > graph;
	std::vector<Node*> v;
	size_t row(1), max(0);


	f.open("_p067_triangle.txt", std::fstream::in);


	if (f.is_open())
	{
		std::string line;
		std::vector<size_t> numVec;

		// Read the file and build the graph
		while (std::getline(f, line))
		{
			parseStrToVec(numVec, line);

			for (auto num : numVec)
			{
				Node* n = new Node();
				n->data = (int)num;
				v.push_back(n);
			}

			if (v.size() == row)
			{
				if (row > 1)
				{
					std::vector<Node*>* prev = &(graph.back());

					for (size_t j = 0; j < prev->size(); j++)
					{
						Node* n = prev->at(j);
						n->left = v[j + 0];
						n->right = v[j + 1];

						size_t lSum = n->sum + n->left->data;
						size_t rSum = n->sum + n->right->data;

						n->left ->sum = lSum < n->left ->sum ? n->left ->sum : lSum;
						n->right->sum = rSum < n->right->sum ? n->right->sum : rSum;
					}
				}
				else
				{
					v[0]->sum = v[0]->data;
				}

				graph.push_back(v);
				v.clear();
				row++;
			}
		}

		f.close();

		// Find max value in the last vector
		for (auto* node : graph.back())
			if (node->sum > max)
				max = node->sum;


		// Clear the graph
		for (auto& vec : graph)
			for (auto* node : vec)
				delete node;
	}

	std::cout << "\n res = " << max << std::endl;

	validateResult(answer, max);
}

// -----------------------------------------------------------------------------------------------
