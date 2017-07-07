#include "CombPrinter.h"
#include "CombStats.h"
#include "MCMCcontrol.h"
#include "patch.h"
#include "MemoryMng.h"
#include "McRefCommon.h"

void printCombStatsHeader(FILE *file) {
  fprintf(file, "iteration\t");
  for (int comb = 0; comb < dataSetup.popTree->numPops; comb++) {
    if (isFeasibleComb(comb)) {
      printOneCombHeader(comb, file);
    }
  }

  for (int pop = 0; pop < dataSetup.popTree->numPops; pop++) {
    printOnePopHeader(pop, file);
  }

  for (int migband = 0; migband < dataSetup.popTree->numMigBands; migband++) {
    printOneMigBandHeader(migband, file);
  }

  fprintf(file, "\n");
}

void printOneCombHeader(int comb, FILE *file) {
  char *combName = getPopName(comb);

  fprintf(file, "C_%s cs\tC_%s nc\t", combName, combName);

  printCombPopHeaders(comb, combName, file);
  printCombMigHeaders(comb, combName, file);

}

void printCombPopHeaders(int comb, char *combName, FILE *file) {
  for (int pop = 0; pop < dataSetup.popTree->numPops; pop++) {
    if (isLeaf(pop) && isAncestralTo(comb, pop)) {
      char *leafName = getPopName(pop);
      fprintf(file, "C_%s_%s cs\tC_%s_%s nc\t",
              combName, leafName, combName, leafName);
    }
  }
}

void printCombMigHeaders(int comb, char *combName, FILE *file) {
  for (int mig = 0; mig < dataSetup.popTree->numMigBands; mig++) {
    if (isCombLeafMigBand(mig, comb)) {
      int source = getSourcePop(mig);
      int target = getTargetPop(mig);
      char *sourceName = getPopName(source);
      char *targetName = getPopName(target);
      fprintf(file, "C_%s_%s->%s ms\tC_%s_%s->%s nm\t",
              combName, sourceName, targetName, combName, sourceName, targetName);
    }
  }
}

void printOnePopHeader(int pop, FILE *file) {
  char *popName = dataSetup.popTree->popArray[pop].name;
  fprintf(file, "P_%s %s\tP_%s %s\t",
          popName, "cs",
          popName, "nc");
}

void printOneMigBandHeader(int migband, FILE *file) {
  char *source = getPopName(dataSetup.popTree->migBands[migband].sourcePop);
  char *target = getPopName(dataSetup.popTree->migBands[migband].targetPop);
  fprintf(file, "MB_%s->%s ms\tMB_%s->%s nm\t",
          source, target,
          source, target);
}


void printCombStats(int iteration, FILE *file) {

  fprintf(file, "%d\t", iteration);
  for (int comb = 0; comb < dataSetup.popTree->numPops; comb++) {
    if (isFeasibleComb(comb)) {
      printOneCombStats(comb, file);
    }
  }

  for (int pop = 0; pop < dataSetup.popTree->numPops; pop++) {
    printOnePopStats(pop, file);
  }


  for (int migband = 0; migband < dataSetup.popTree->numMigBands; migband++) {
    printOneMigBandStats(migband, file);
  }

  fprintf(file, "\n");
  fflush(file);
}

void printOneCombStats(int comb, FILE *file) {
  fprintf(file, "%0.35f\t%d\t",  //TODO - turn float precision to a MACRO/CONST
          comb_stats[comb].total.coal_stats,
          comb_stats[comb].total.num_coals);
  printCombCoalStats(comb, file);
  printCombMigStats(comb, file);
}

void printCombCoalStats(int comb, FILE *file) {
  for (int pop = 0; pop < dataSetup.popTree->numPops; pop++) {
    if (isLeaf(pop) && isAncestralTo(comb, pop)) {
      fprintf(file, "%0.35f\t%d\t",
              comb_stats[comb].leaves[pop].below_comb.coal_stats,
              comb_stats[comb].leaves[pop].below_comb.num_coals);
    }
  }
}

void printCombMigStats(int comb, FILE *file) {
  for (int mig = 0; mig < dataSetup.popTree->numMigBands; mig++) {
    if (isCombLeafMigBand(mig, comb)) {
      fprintf(file, "%0.35f\t%d\t",
              comb_stats[comb].leafMigs[mig].mig_stats,
              comb_stats[comb].leafMigs[mig].num_migs);
    }
  }
}

void printOnePopStats(int pop, FILE *file) {
  fprintf(file, "%0.35f\t%d\t",
          genetree_stats_total.coal_stats[pop],
          genetree_stats_total.num_coals[pop]);
}

void printOneMigBandStats(int migband, FILE *file) {
  fprintf(file, "%0.35f\t%d\t",
          genetree_stats_total.mig_stats[migband],
          genetree_stats_total.num_migs[migband]);
}
