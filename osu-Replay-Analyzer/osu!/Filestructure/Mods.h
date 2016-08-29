#ifndef MODS_H
#define MODS_H

class Mods
{
	struct Modifier
	{
		// visual modifiers
		bool HD = false; // shows then hides hides notes before hitting
		bool FI = false; // shows notes right before hitting (mania)
		bool FL = false; // shows a limited section of the play field
		bool RV = false; // flips the notes on a diagonal x-y axis

						 // difficulty modifiers
		bool HT = false; // Half time	(-TM)
		bool DT = false; // Double time (+TM)
		bool EZ = false; // Easy		(-CS, -OD, -AR)
		bool HR = false; // Hard Rock	(+CS, +OD, +AR)
	};


	public:
		enum MODS
		{
			NM = 0 << 0,
			NF = 1 << 0,
			EZ = 1 << 1,
			// video mod no longer exists
			HD = 1 << 3,
			HR = 1 << 4,
			SD = 1 << 5,
			DT = 1 << 6,
			RL = 1 << 7,
			HT = 1 << 8,
			NC = 1 << 9,
			FL = 1 << 10,
			AU = 1 << 11,
			SO = 1 << 12,
			AP = 1 << 13,
			PF = 1 << 14,
			K4 = 1 << 15,
			K5 = 1 << 16,
			K6 = 1 << 17,
			K7 = 1 << 18,
			K8 = 1 << 19,
			FI = 1 << 20,
			RD = 1 << 21,
			// Cinema
			// Taget practice
			K9 = 1 << 24,
			CP = 1 << 25,
			// ???
		};

		Mods();

		void setModifier(MODS _mod);
		Modifier getModifier();

		void setAR(double _AR);
		void setCS(double _CS);
		void setOD(double _OD);
		void setTM(double _TM);

		double getAR();
		double getCS();
		double getOD();
		double getTM();

	private:
		void ApplyDiffMods();

		Modifier mod;

		double AR;	// Approach rate
		double CS;	// Circle Size
		double OD;	// Overall Difficulty (how precise the hits need to be)
		double TM;  // Time multiplier; how faster the map is
};

#endif // !MODS_H