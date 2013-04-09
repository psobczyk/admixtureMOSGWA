#ifndef IO_PLINKINPUT_HPP
#define IO_PLINKINPUT_HPP

#include "Input.hpp"
#include <vector>
#include "../linalg/AutoVector.hpp"
#include "../linalg/AutoMatrix.hpp"

namespace io {

	/** Reads input data from PLink BIM, FAM and BED formatted files. */
	class PlinkInput : public Input {

		/** File extensions for the Plink files for SNPs, Individuals and genome. */
		static const char
			* const snpListExtension,
			* const individualListExtension,
			* const genotypeMatrixExtension;

		/** Translation table from two genome bits to the number for the regression matrix entry. */
		static const double genotypeTranslation[];

		/** Stores all SNP characteristics in memory. */
		std::vector<SNP> snpList;

		/** Stores all Individual characteristics in memory. */
		std::vector<Individual> individualList;

		/** Genome data matrix.
		* It is stored as transposed matrix in order to optimise memory access.
		* The vectors holding all individuals' data for one SNP should be in a contiguous piece of memory
		* so that the whole vector fits into cache.
		* It is these vectors which are added to the regression algorithm in the search phase.
		*/
		linalg::AutoMatrix genotypeMatrixTransposed;

		/** Phenotype data vector. */
		linalg::AutoVector phenotypeVector;

		public:

		/** Set up the Plink file reading. */
		PlinkInput ( const char* const filename );

		/** Return the number of SNPs in the data. */
		virtual size_t countSnps () const;

		/** Return the number of individuals in the data. */
		virtual size_t countIndividuals () const;

		/** Retrieve the data for the given SNP. */
		virtual SNP getSnp ( const size_t snpIndex );

		/** Retrieve the data for the given individual. */
		virtual Individual getIndividual ( const size_t individualIndex );

		/** Retrieve the {@link countIndividuals} sized vector of genotype information for the given SNP. */
		virtual linalg::Vector getGenotypeVector ( const size_t snpIndex );

		/** Retrieve the {@link countIndividuals} sized vector of phenotype information. */
		virtual linalg::Vector getPhenotypeVector ();

		/** Declare access to be finished, release all resources. */
		virtual ~PlinkInput ();
	};

}

#endif	/* IO_PLINKINPUT_HPP */
